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

    struct WaitResult {
        bool dataToRead {0};
        bool urgentDataToRead {0};
        bool writingWillNotBlock {0};
        bool readHangup {0};
        bool writeHangup {0};
        bool error {0};      
        bool invalid {0};
    };

    PosixSocket(ProtocolDomain protocolDomain, Type type, FrameType frameType, bool nonBlocking);
    PosixSocket(const PosixSocket&) = delete;
    ~PosixSocket();

    static outcome::result<std::array<std::shared_ptr<PosixSocket>, 2>> createUnnamedPair(ProtocolDomain protocolDomain, Type type, FrameType frameType, bool nonBlocking);

    outcome::result<void> bind(std::string url);
    outcome::result<void> connect(std::string url);

    // TODO add wakeup to interrupt wait -> multithreading??
    outcome::result<WaitResult> poll(std::chrono::milliseconds timeout, bool writePending);

    outcome::result<ssize_t> read(const gsl::span<std::byte>& buffer);
    outcome::result<ssize_t> write(const gsl::span<std::byte>& buffer);

    outcome::result<ssize_t> receiveFrom(const gsl::span<std::byte>& buffer);
    outcome::result<ssize_t> sendTo(std::string url, const gsl::span<std::byte>& buffer);

    outcome::result<void> close();
private:
    outcome::result<void> create();

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
