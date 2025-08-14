/***
 * Copyright 2019 The Katla Authors
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "posix-socket.h"

#include <asm-generic/errno.h>
#include <sys/poll.h>
#include <vector>

#include <unistd.h>
#include <cstring>
#include <system_error>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/eventfd.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include <net/ethernet.h>
#include <net/if.h>

#include <arpa/inet.h>

#include <poll.h>

namespace katla {

PosixSocket::PosixSocket()
{
}

PosixSocket::PosixSocket(ProtocolDomain protocolDomain, Type type, FrameType frameType, bool nonBlocking) :
    _protocolDomain(protocolDomain),
    _type(type),
    _frameType(frameType),
    _nonBlocking(nonBlocking)
{
}

PosixSocket::PosixSocket(ProtocolDomain protocolDomain, Type type, FrameType frameType, bool nonBlocking, int fd, int wakeupFd) :
        _fd(fd),
        _wakeupFd(wakeupFd),
        _protocolDomain(protocolDomain),
        _type(type),
        _frameType(frameType),
        _nonBlocking(nonBlocking)
{
}

PosixSocket::~PosixSocket()
{
    if (_wakeupFd != -1) {
        ::close(_wakeupFd);
    }

    if (_fd != -1) {
        ::close(_fd);
    }
}

katla::result<std::array<std::shared_ptr<PosixSocket>,2>, Error> PosixSocket::createUnnamedPair(ProtocolDomain protocolDomain, Type type, FrameType frameType, bool nonBlocking)
{
    PosixErrorCategory errorCategory;

    int mappedDomain = mapProtocolDomain(protocolDomain);
    if (mappedDomain == -1) {
        return Error(make_error_code(PosixErrorCodes::InvalidDomain));
    }

    int mappedType = mapType(type);
    if (mappedType == -1) {
        return Error(make_error_code(PosixErrorCodes::InvalidType));
    }

    if (nonBlocking) {
        mappedType |= SOCK_NONBLOCK;
    }

    int sd[2] = {-1,-1};
    int result = socketpair(mappedDomain, mappedType, 0, sd);
    if (result != 0) {
        return Error(std::make_error_code(static_cast<std::errc>(errno)), "Failed creating socket pair");
    }

    int wakeupFd[2] = {-1,-1};
    wakeupFd[0] = eventfd(0, 0);
    if (wakeupFd[0] == -1) {
        ::close(sd[0]);
        ::close(sd[1]);
        return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed creating wakeup fd"));
    }

    wakeupFd[1] = eventfd(0, 0);
    if (wakeupFd[1] == -1) {
        ::close(sd[0]);
        ::close(sd[1]);
        ::close(wakeupFd[0]);
        return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed creating wakeup fd"));
    }

    return outcome::success(std::array<std::shared_ptr<PosixSocket>,2>{
            std::shared_ptr<PosixSocket>(new PosixSocket(protocolDomain, type, frameType, nonBlocking, sd[0], wakeupFd[0])),
            std::shared_ptr<PosixSocket>(new PosixSocket(protocolDomain, type, frameType, nonBlocking, sd[1], wakeupFd[1]))
    });
}

int PosixSocket::mapProtocolDomain(ProtocolDomain protocolDomain) {
    switch(protocolDomain) {
        case ProtocolDomain::Unix: return AF_UNIX;
        case ProtocolDomain::IPv4: return AF_INET;
        case ProtocolDomain::IPv6: return AF_INET6;
        case ProtocolDomain::Packet: return AF_PACKET;
        case ProtocolDomain::Can: return AF_CAN;
        case ProtocolDomain::Bluetooth: return AF_BLUETOOTH;
        case ProtocolDomain::VSock: return AF_VSOCK;
    }

    return -1;
}

int PosixSocket::mapType(Type type) {
    switch(type) {
        case Type::Stream: return SOCK_STREAM;
        case Type::Datagram: return SOCK_DGRAM;
        case Type::SequencedPacket: return SOCK_SEQPACKET;
        case Type::Raw: return SOCK_RAW;
    }

    return -1;
}

katla::result<void, Error> PosixSocket::bind(std::string url)
{
    if (_protocolDomain == ProtocolDomain::Packet && _type == Type::Raw) {
        auto result = create();
        if (!result) {
            return result.error();
        }

        auto nameToIndexResult = if_nametoindex(url.c_str());
        if (nameToIndexResult == 0) {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed finding adapter: {}: {}\n", nameToIndexResult, url));
        }

        sockaddr_ll destAddress = {};
        destAddress.sll_family = AF_PACKET;
        destAddress.sll_protocol = htons( static_cast<uint16_t> (_frameType));
        destAddress.sll_ifindex = static_cast<int>(nameToIndexResult);
        destAddress.sll_pkttype = PACKET_MULTICAST;

        auto bindResult = ::bind(_fd, reinterpret_cast<sockaddr*>(&destAddress), sizeof(destAddress));
        if (bindResult == -1) {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed binding adapter: {}: {}\n", nameToIndexResult, url));
        }

        _url = url;

        struct packet_mreq mreq = {};
        mreq.mr_ifindex = static_cast<int>(nameToIndexResult);
        mreq.mr_type = PACKET_MR_PROMISC;
        if (setsockopt(_fd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) == -1) {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed setting socket options on: {}: {}\n", nameToIndexResult, url));
        }

        return outcome::success();
    }
    
    if (_protocolDomain == ProtocolDomain::Unix) {
        auto result = create();
        if (!result) {
            return result.error();
        }

        if (url.length() >= 108) {
            return make_error_code(PosixErrorCodes::UnixSocketPathTooLong);
        }

        sockaddr_un bindAddress = {};
        bindAddress.sun_family = AF_UNIX;
        strncpy(bindAddress.sun_path, url.c_str(), url.length() + 1);

        auto bindResult = ::bind(_fd, reinterpret_cast<sockaddr*>(&bindAddress), sizeof(bindAddress));
        if (bindResult == -1) {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed binding to path: {}\n", url));
        }

        return outcome::success();
    } 

    return make_error_code(PosixErrorCodes::OperationNotSupported);
}

katla::result<void, Error> PosixSocket::bindIPv4(std::string ip, int port, SocketOptions options)
{
    if (_protocolDomain == ProtocolDomain::IPv4 && _type == Type::Stream) {
        auto result = create();
        if (!result) {
            return result.error();
        }

        int setsockoptEnable = options.reuseAddress ? 1 : 0;
        if (setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &setsockoptEnable, sizeof(int)) < 0)
        {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed opening socket: {}:{}\n", ip, port));
        }

        sockaddr_in address = {};
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(port);

        int ipConvResult = inet_aton(ip.c_str(), &address.sin_addr);
        if (ipConvResult == -1) {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed parsing ip address: {}\n", ip));
        }

        auto bindResult = ::bind(_fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
        if (bindResult == -1) {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed binding to port: {}:{}\n", ip, port));
        }

        return outcome::success();
    }

    return make_error_code(PosixErrorCodes::OperationNotSupported);
}

katla::result<void, Error> PosixSocket::listen()
{
    if (_type != Type::Stream && _type != Type::SequencedPacket) {
        return make_error_code(PosixErrorCodes::OperationNotSupported);
    }

    auto listenResult = ::listen(_fd, 1);
    if (listenResult == -1) {
        return Error(
            std::make_error_code(static_cast<std::errc>(errno)),
            katla::format("Failed listening to socket"));
    }

    return outcome::success();
}

katla::result<std::unique_ptr<PosixSocket>, Error> PosixSocket::accept()
{
    if (_type != Type::Stream && _type != Type::SequencedPacket) {
        return make_error_code(PosixErrorCodes::OperationNotSupported);
    }

    socklen_t addressLength = sizeof(sockaddr_in);
    struct sockaddr_in address; ///two objects to store client and server address

    auto acceptResult = ::accept(_fd, (struct sockaddr *) &address, &addressLength);
    if (acceptResult == -1) {
        return Error(
            std::make_error_code(static_cast<std::errc>(errno)),
            katla::format("Failed accepting client socket"));
    }

    auto wakeupFd = eventfd(0, 0);
    if (wakeupFd == -1) {
        ::close(acceptResult);
        return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed creating wakeup fd"));
    };

    auto clientSocket = std::unique_ptr<PosixSocket>(new PosixSocket(_protocolDomain, _type, _frameType, _nonBlocking, acceptResult, wakeupFd));
    return clientSocket;
}

std::optional<Error> PosixSocket::error()
{
    int errorValue = 0;
    socklen_t errorValueLength = sizeof(errorValue);
    auto result = ::getsockopt(_fd, SOL_SOCKET, SO_ERROR, &errorValue, &errorValueLength);
    if (result == -1) {
        return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed getting socket error"));
    }

    if (errorValue) {
        return Error(
                std::make_error_code(static_cast<std::errc>(errorValue)),
                katla::format("Socket error"));
    }

    return {};
}

katla::result<void, Error> PosixSocket::connect(std::string url, SocketOptions options)
{
    if (_protocolDomain == ProtocolDomain::Unix) {
        auto result = create();
        if (!result) {
            return result.error();
        }

        if (url.length() >= 108) {
            return make_error_code(PosixErrorCodes::UnixSocketPathTooLong);
        }

        _nonBlocking = options.nonBlocking;

        sockaddr_un bindAddress = {};
        bindAddress.sun_family = AF_UNIX;
        strncpy(bindAddress.sun_path, url.c_str(), url.length() + 1);

        auto connectResult = ::connect(_fd, reinterpret_cast<sockaddr*>(&bindAddress), sizeof(bindAddress));
        if (connectResult == -1 && connectResult != EINPROGRESS) {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed connecting to path: {}\n", url));
        }

        return outcome::success();
    }

    return make_error_code(PosixErrorCodes::OperationNotSupported);
}

katla::result<void, Error> PosixSocket::connectIPv4(std::string ip, int port, SocketOptions options)
{
    if (_protocolDomain == ProtocolDomain::IPv4) {
        auto result = create();
        if (!result) {
            return result.error();
        }

        _nonBlocking = options.nonBlocking;

        sockaddr_in address = {};
        address.sin_family = AF_INET;
        address.sin_port = htons(port);

        int ipConvResult = inet_aton(ip.c_str(), &address.sin_addr);
        if (ipConvResult == -1) {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed parsing ip address: {}\n", ip));
        }

        auto connectResult = ::connect(_fd, reinterpret_cast<sockaddr*>(&address), sizeof(address));
        if (connectResult == -1 && connectResult != EINPROGRESS) {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed connecting to ip: {}:{}\n", ip, port));
        }

        return outcome::success();
    }

    return make_error_code(PosixErrorCodes::OperationNotSupported);
}


katla::result<PosixSocket::WaitResult, Error> PosixSocket::poll(std::chrono::milliseconds timeout, bool writePending)
{
    pollfd pollDescriptor[2] = {{
        _fd,
        POLLIN | POLLPRI | POLLRDHUP,
        0
    }, {
        _wakeupFd,
        POLLIN,
        0
    }};

    // If we have bytes to send we want to return early to send our bytes
    if (writePending) {
        pollDescriptor[0].events |= POLLOUT;
    }

    auto result = ::poll(pollDescriptor, 2, static_cast<int>(timeout.count()));
    if (result == -1) {
        return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed polling socket"));
    }

    WaitResult waitResult;
    waitResult.dataToRead = (pollDescriptor[0].revents & POLLIN) || (pollDescriptor[0].revents & POLLPRI);
    waitResult.urgentDataToRead = (pollDescriptor[0].revents & POLLPRI);
    waitResult.writingWillNotBlock = (pollDescriptor[0].revents & POLLOUT);
    waitResult.readHangup = (pollDescriptor[0].revents & POLLRDHUP);
    waitResult.writeHangup = (pollDescriptor[0].revents & POLLHUP);
    waitResult.error = (pollDescriptor[0].revents & POLLERR);
    waitResult.invalid = (pollDescriptor[0].revents & POLLNVAL);
    waitResult.wakeup = (pollDescriptor[1].revents & POLLIN);
    
    if (waitResult.wakeup) 
    {   //read data written to _wakeupFd to prevent subsequent wakeups without request
        int size = sizeof(uint64_t);
        std::vector<std::byte> buffer(size, std::byte{0});

        auto readResult = ::read(_wakeupFd, buffer.data(), buffer.size());
        if (readResult == -1) {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed reading socket"));
        }
    }

    return waitResult;
}

katla::result<ssize_t, Error> PosixSocket::read(const katla::span<std::byte>& buffer)
{
    return receiveFrom(buffer);
}

katla::result<ssize_t, Error> PosixSocket::receiveFrom(const katla::span<std::byte>& buffer)
{
    int flags = 0;
    if (_nonBlocking) {
        flags |= MSG_DONTWAIT;
    }

    ssize_t nbytes = ::recvfrom(_fd, buffer.data(), buffer.size(), flags, nullptr, nullptr);
    if (nbytes == -1) {
        if (_nonBlocking && (errno == EAGAIN || errno == EWOULDBLOCK)) {
            // TODO TEST??
            nbytes = 0;
        } else {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed receving from socket"));
        }
    }

    return nbytes;
}

katla::result<ssize_t, Error> PosixSocket::write(const katla::span<std::byte>& buffer)
{
    ssize_t nbytes = ::write(_fd, buffer.data(), buffer.size());

    if (nbytes == -1) {
        return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed writing to socket"));
    }

    return nbytes;
}

katla::result<ssize_t, Error> PosixSocket::sendTo(std::string url, const katla::span<std::byte>& buffer)
{
    if (_protocolDomain == ProtocolDomain::Packet && _type == Type::Raw) {
        auto nameToIndexResult = if_nametoindex(url.c_str());
        if (nameToIndexResult == 0) {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed finding adapter: {}: {}\n", nameToIndexResult, url));
        }

        sockaddr_ll destAddress = {};
        destAddress.sll_family = AF_PACKET;
        destAddress.sll_protocol = htons( static_cast<uint16_t> (_frameType));
        destAddress.sll_ifindex = nameToIndexResult;

        std::array<uint8_t, 8> addr = {1,1,5,4,0,0};
        ::memcpy(destAddress.sll_addr, addr.data(), addr.size());
        destAddress.sll_halen = 6;

        ssize_t nbytes = ::sendto(_fd, buffer.data(), buffer.size(), 0, reinterpret_cast<sockaddr*>(&destAddress), sizeof(sockaddr_ll));
        return nbytes;

        if (nbytes == -1) {
            return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed sending to socket"));
        }
    }
    
    return make_error_code(PosixErrorCodes::OperationNotSupported);
}

katla::result<void, Error> PosixSocket::wakeup()
{
    uint64_t counter = 1;
    int status = ::write(_wakeupFd, reinterpret_cast<void*>(&counter), sizeof(counter));
    if (status == -1) {
        return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed to wakeup socket"));
    }

    return outcome::success();
}

katla::result<void, Error> PosixSocket::close()
{
    if (_fd == -1) {
        return outcome::success();
    }

    int status = ::close(_fd);
    if (status == -1) {
        return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed closing socket"));
    }

    _fd = -1;

    return outcome::success();
}

katla::result<void, Error> PosixSocket::create()
{
    int domain = mapProtocolDomain(_protocolDomain);
    if (domain == -1) {
        return Error(make_error_code(PosixErrorCodes::InvalidDomain));
    }

    int mappedType = mapType(_type);
    if (mappedType == -1) {
        return Error(make_error_code(PosixErrorCodes::InvalidType));
    }

    if (_nonBlocking) {
        mappedType |= SOCK_NONBLOCK;
    }

    // Only set frame/protocol type for RAW sockets for now
    uint16_t frameType = 0;
    if (_type == Type::Raw) {
        frameType = static_cast<uint16_t> ( htons( frameType ));
    }

    _fd = socket(domain, mappedType, frameType);
    if (_fd == -1) {
        // TODO check root access for raw sockets
        return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed creating socket"));
    }

    _wakeupFd = eventfd(0, 0);
    if (_wakeupFd == -1) {
        return Error(
                std::make_error_code(static_cast<std::errc>(errno)),
                katla::format("Failed creating wakeup fd"));
    }

    return outcome::success();
}

}
