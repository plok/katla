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

#ifndef KATLA_SOCKET_H
#define KATLA_SOCKET_H

#include "posix-errors.h"

#include "katla/core/error.h"
#include "katla/core/core.h"

#include <gsl/span>

#include <net/ethernet.h>

#include <memory>
#include <optional>
#include <system_error>

#include <chrono>
#include <string>

namespace outcome = OUTCOME_V2_NAMESPACE;

namespace katla {

class PosixSocket {
public:
    enum class ProtocolDomain { Unix, IPv4, IPv6, Packet, Can, Bluetooth, VSock};
    enum class Type { Stream, Datagram, SequencedPacket, Raw};
    enum class FrameType : uint16_t { All = ETH_P_ALL, EtherCat = 0x88a4 };

    struct SocketOptions {
        ProtocolDomain domain {ProtocolDomain::IPv4};
        Type type {Type::Stream};
        FrameType frameType {FrameType::All};
        bool nonBlocking {false};
    };

    static constexpr SocketOptions IPv4SocketOptions {ProtocolDomain::IPv4, Type::Stream, FrameType::All, false};

    struct WaitResult {
        bool dataToRead {0};
        bool urgentDataToRead {0};
        bool writingWillNotBlock {0};
        bool readHangup {0};
        bool writeHangup {0};
        bool error {0};      
        bool invalid {0};
    };

    PosixSocket();
    PosixSocket(ProtocolDomain protocolDomain, Type type, FrameType frameType, bool nonBlocking);
    PosixSocket(const PosixSocket&) = delete;
    ~PosixSocket();

    static outcome::result<std::array<std::shared_ptr<PosixSocket>, 2>, Error> createUnnamedPair(ProtocolDomain protocolDomain, Type type, FrameType frameType, bool nonBlocking);

    outcome::result<void, Error> bind(std::string url);
    outcome::result<void, Error> connect(std::string url, SocketOptions options);
    outcome::result<void, Error> connectIPv4(std::string ip, int port, SocketOptions options = IPv4SocketOptions);

    // TODO add wakeup to interrupt wait -> multithreading??
    outcome::result<WaitResult, Error> poll(std::chrono::milliseconds timeout, bool writePending);

    outcome::result<ssize_t, Error> read(const gsl::span<std::byte>& buffer);
    outcome::result<ssize_t, Error> write(const gsl::span<std::byte>& buffer);

    outcome::result<ssize_t, Error> receiveFrom(const gsl::span<std::byte>& buffer);
    outcome::result<ssize_t, Error> sendTo(std::string url, const gsl::span<std::byte>& buffer);

    outcome::result<void, Error> close();
private:
    outcome::result<void, Error> create();

    PosixSocket & operator=(const PosixSocket&) = delete;

    PosixSocket(ProtocolDomain protocolDomain, Type type, FrameType frameType, bool nonBlocking, int fd);

    static int mapProtocolDomain (ProtocolDomain protocolDomain);
    static int mapType (Type type);

    int _fd {-1};

    ProtocolDomain _protocolDomain {ProtocolDomain::IPv4};
    Type _type {Type::Stream};
    FrameType _frameType {FrameType::All};

    std::string _url;

    bool _nonBlocking {false};
};

}

#endif // KATLA_SOCKET_H
