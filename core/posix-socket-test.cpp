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

#include "core/posix-socket.h"

#include "gtest/gtest.h"

#include <gsl/span>

#include <fmt/format.h>
#include <variant>
#include <chrono>

#include <utility>
#include <memory>
#include <functional>

#include <cstdlib>
#include <unistd.h>
#include <exception>
#include <fmt/format.h>
#include <variant>
#include <chrono>

namespace katla {

    std::string helloWorld = "Hello World!";

    void testChild(PosixSocket &socket) {
        fmt::print("child: Starting..\n");

        gsl::span<std::byte> messageSpan(reinterpret_cast<std::byte *>(helloWorld.data()), helloWorld.size());

        std::vector<std::byte> sendBuffer;
        sendBuffer.insert(sendBuffer.end(), messageSpan.begin(), messageSpan.end());

        gsl::span<std::byte> sendSpan(sendBuffer);

        std::function<void(gsl::span<std::byte>)> sendFunc = [&socket](gsl::span<std::byte> frame) {
            auto result = socket.write(frame);
            if (result) {
                fmt::print("written bytes {}\n", result.value());

                if (result.value() != static_cast<ssize_t>(frame.size())) {
                    fmt::print("warning: incomplete write!\n", result.value());
                }
            } else {
                fmt::print("failed writing to pipe: {}\n", result.error().message());
            }
        };

        fmt::print("send!\n");

        sendFunc(sendSpan);

        fmt::print("child: closed..\n");
    }

    void testParent(PosixSocket &socket) {
        fmt::print("parent: starting..\n");

        const int BUFFER_SIZE = 100;
        std::vector<std::byte> buffer(BUFFER_SIZE, std::byte{0});

        std::function<std::chrono::milliseconds()> getTimestamp = []() {
            return std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::system_clock::now().time_since_epoch());
        };

        auto startTime = getTimestamp();

        bool done = false;
        while (!done) {
            gsl::span<std::byte> bufferSpan(buffer.data(), buffer.size());
            auto result = socket.read(bufferSpan);
            if (!result) {
                fmt::print(stderr, "server: failed reading from pipe with error: {0}!\n", strerror(errno));
            }

            gsl::span<std::byte> readSpan(buffer.data(), result.value());

            if (readSpan.size() == helloWorld.length()) {
                fmt::print("Frame received!\n");
                GTEST_SUCCEED();
                done = true;
                break;
            }

            auto timeoutElapsed = (getTimestamp() - startTime) > std::chrono::milliseconds(4000);
            ASSERT_EQ(timeoutElapsed, false); //timeout
        }

        fmt::print("parent: closed...\n");
    }

/***
 * - parent starts listening
 * - child sends message
 * - parent receives message
 */
    TEST(KatlaIpcTests, UnixSocketPairTest) {

        auto createResult = PosixSocket::createUnnamedPair(PosixSocket::ProtocolDomain::Unix, PosixSocket::Type::Datagram, PosixSocket::FrameType::All, false);
        ASSERT_TRUE(createResult) << createResult.error().message();

        auto parentSocket = createResult.value()[0];
        auto childSocket = createResult.value()[1];

        auto forkResult = fork();
        if (forkResult == -1) {
            ASSERT_NE(forkResult, -1);
        }

        if (forkResult == 0) {
            // child
            auto result = parentSocket->close();
            ASSERT_TRUE(result) << result.error().message();

            testChild(*childSocket);

            result = childSocket->close();
            ASSERT_TRUE(result) << result.error().message();

            exit(EXIT_SUCCESS);
        }

        // parent
        auto result = childSocket->close();
        ASSERT_TRUE(result) << result.error().message();

        testParent(*parentSocket);

        result = parentSocket->close();
        ASSERT_TRUE(result) << result.error().message();
    }

    outcome::result<std::string> createTemporaryDir() {
        std::string dirTemplate = "/tmp/katla-test-XXXXXX";
        if (mkdtemp(dirTemplate.data()) == NULL) {
            return std::make_error_code(static_cast<std::errc>(errno));
        }
        return dirTemplate;
    }

    TEST(KatlaIpcTests, UnixSocketDatagramTest) {
        auto tmpDir = createTemporaryDir();
        ASSERT_TRUE(tmpDir) << tmpDir.error().message();

        fmt::print("{}\n", tmpDir.value());
        fflush(stdout);

        auto url = fmt::format("{}/test.sock", tmpDir.value());

        auto forkResult = fork();
        if (forkResult == -1) {
            ASSERT_NE(forkResult, -1);
        }

        if (forkResult == 0) {
            // child
            PosixSocket childSocket(PosixSocket::ProtocolDomain::Unix, PosixSocket::Type::Datagram, PosixSocket::FrameType::All, false);
            auto connectResult = childSocket.connect(url);
            ASSERT_TRUE(connectResult) << connectResult.error().message();

            testChild(childSocket);

            auto closeResult = childSocket.close();
            ASSERT_TRUE(closeResult) << closeResult.error().message();

            exit(EXIT_SUCCESS);
        }

        // parent
        PosixSocket parentSocket(PosixSocket::ProtocolDomain::Unix, PosixSocket::Type::Datagram, PosixSocket::FrameType::All, false);

        auto bindResult = parentSocket.bind(url);
        ASSERT_TRUE(bindResult) << bindResult.error().message();

        testParent(parentSocket);

        auto result = parentSocket.close();
        ASSERT_TRUE(result) << result.error().message();
    }

}

