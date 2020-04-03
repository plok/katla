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

#include "fmt/format.h"

#include <vector>

#include <unistd.h>
#include <cstring>
#include <system_error>

#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>

#include <net/ethernet.h>
#include <net/if.h>

#include <arpa/inet.h>

namespace katla {

PosixSocket::PosixSocket(ProtocolDomain protocolDomain, Type type, FrameType frameType, bool nonBlocking) :
    _protocolDomain(protocolDomain),
    _type(type),
    _frameType(frameType),
    _nonBlocking(nonBlocking),
    _fd(-1)
{
}

PosixSocket::PosixSocket(ProtocolDomain protocolDomain, Type type, FrameType frameType, bool nonBlocking, int fd) :
        _protocolDomain(protocolDomain),
        _type(type),
        _frameType(frameType),
        _nonBlocking(nonBlocking),
        _fd(fd)
{
}

PosixSocket::~PosixSocket()
{
    if (_fd != -1) {
        ::close(_fd);
    }
}

outcome::result<std::array<std::shared_ptr<PosixSocket>,2>> PosixSocket::createUnnamedPair(ProtocolDomain protocolDomain, Type type, FrameType frameType, bool nonBlocking)
{
    PosixErrorCategory errorCategory;

    int mappedDomain = mapProtocolDomain(protocolDomain);
    if (mappedDomain == -1) {
        std::error_code errorCode(static_cast<int>(PosixErrorCodes::InvalidDomain), errorCategory);
        return errorCode;
    }

    int mappedType = mapType(type);
    if (mappedType == -1) {
        std::error_code errorCode(static_cast<int>(PosixErrorCodes::InvalidType), errorCategory);
        return errorCode;
    }

    if (nonBlocking) {
        mappedType |= SOCK_NONBLOCK;
    }

    int sd[2] = {-1,-1};
    int result = socketpair(mappedDomain, mappedType, 0, sd);
    if (result != 0) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return outcome::success(std::array<std::shared_ptr<PosixSocket>,2>{
            std::shared_ptr<PosixSocket>(new PosixSocket(protocolDomain, type, frameType, nonBlocking, sd[0])),
            std::shared_ptr<PosixSocket>(new PosixSocket(protocolDomain, type, frameType, nonBlocking, sd[1]))
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

outcome::result<void> PosixSocket::open(int protocol)
{
    int domain = mapProtocolDomain(_protocolDomain);
    if (domain == -1) {
        return make_error_code(PosixErrorCodes::InvalidDomain);
    }

    int mappedType = mapType(_type);
    if (mappedType == -1) {
        return make_error_code(PosixErrorCodes::InvalidType);
    }

    if (_nonBlocking) {
        mappedType |= SOCK_NONBLOCK;
    }

    uint16_t nsFrameType = ::htons( static_cast<uint16_t> (_frameType) );

    fmt::print("{} {}\n", _frameType, nsFrameType);
    fflush(stdout);

    _fd = socket(domain, mappedType, nsFrameType);
    if (_fd == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    sockaddr_ll destAddress = {};
    destAddress.sll_family = AF_PACKET;
    destAddress.sll_protocol = ::htons( static_cast<uint16_t> (_frameType));
    destAddress.sll_ifindex = 2; // virbr0 // TODO make configurable
    destAddress.sll_pkttype = PACKET_MULTICAST;

    auto bindResult = ::bind(_fd, reinterpret_cast<sockaddr*>(&destAddress), sizeof(destAddress));
    if (bindResult == -1) {
        fmt::print("failed bind!");
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    struct packet_mreq mreq = {0};
    mreq.mr_ifindex = 2;
    mreq.mr_type = PACKET_MR_PROMISC;
    if (setsockopt(_fd, SOL_PACKET, PACKET_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    return outcome::success();
}

outcome::result<ssize_t> PosixSocket::read(const absl::Span<std::byte>& buffer)
{
    sockaddr_ll destAddress = {};
    destAddress.sll_family = AF_PACKET;
    destAddress.sll_protocol = ::htons( static_cast<uint16_t> (_frameType));
    destAddress.sll_ifindex = 2; // virbr0 // TODO make configurable
    destAddress.sll_pkttype = PACKET_MULTICAST;

    ssize_t nbytes = ::recvfrom(_fd, buffer.data(), buffer.size(), 0, nullptr, nullptr);

    if (nbytes == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return nbytes;
}

outcome::result<ssize_t> PosixSocket::write(const absl::Span<std::byte>& buffer)
{
    ssize_t nbytes = ::write(_fd, buffer.data(), buffer.size());

    if (nbytes == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return nbytes;
}

outcome::result<ssize_t> PosixSocket::sendto(const absl::Span<std::byte>& buffer)
{
    sockaddr_ll destAddress = {};
    destAddress.sll_family = AF_PACKET;
    destAddress.sll_protocol = ::htons( static_cast<uint16_t> (_frameType));
    destAddress.sll_ifindex = 2; // virbr0 // TODO make configurable

    std::array<uint8_t, 8> addr = {1,1,5,4,0,0};
    ::memcpy(destAddress.sll_addr, addr.data(), addr.size());
    destAddress.sll_halen = 6;

    ssize_t nbytes = ::sendto(_fd, buffer.data(), buffer.size(), 0, reinterpret_cast<sockaddr*>(&destAddress), sizeof(sockaddr_ll));

    if (nbytes == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    return nbytes;
}



outcome::result<void> PosixSocket::close()
{
    if (_fd == -1) {
        return outcome::success();
    }

    int status = ::close(_fd);
    if (status == -1) {
        return std::make_error_code(static_cast<std::errc>(errno));
    }

    _fd = -1;

    return outcome::success();
}

}
