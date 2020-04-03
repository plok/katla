#include "../ipc-kit.h"

#include "app-kit/core-application-uv.h"

#include "uv.h"

#include "../messages.h"
#include "../frame-reader.h"
#include "../pipe.h"

#include "gtest/gtest.h"

#include "absl/types/span.h"

#include <iostream>
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


// Send two frames as stream from child to parent
void framingTestsChild(Pipe& pipeWrite) {
    std::cout << "child: Starting.." << std::endl;
    auto message = std::make_shared<InitialFrame>();
    message->frameHeader.frameLength = sizeof(InitialFrame);
    message->frameHeader.frameNumber = 1;
    message->header.type= FrameType::Initial;
    message->header.version= 0;

    auto message2 = std::make_shared<InitialFrame>();
    message2->frameHeader.frameLength = sizeof(InitialFrame);
    message2->frameHeader.frameNumber = 2;
    message2->header.type= FrameType::Initial;
    message2->header.version= 0;

    absl::Span<std::byte> messageSpan (reinterpret_cast<std::byte*>(message.get()), sizeof(InitialFrame));
    absl::Span<std::byte> message2Span (reinterpret_cast<std::byte*>(message2.get()), sizeof(InitialFrame));

    std::vector<std::byte> sendBuffer;
    sendBuffer.insert(sendBuffer.end(), messageSpan.begin(), messageSpan.end());
    sendBuffer.insert(sendBuffer.end(), message2Span.begin(), message2Span.end());

    absl::Span<std::byte> sendSpan(sendBuffer);

    std::function<void(absl::Span<std::byte>)> sendFunc = [&pipeWrite](absl::Span<std::byte> frame) {
        auto result = pipeWrite.write(frame);
        if (result) {
            std::cout << "written bytes" << result << std::endl;

            if (result.value() != static_cast<ssize_t>(frame.size())) {
                std::cout << "warning: incomplete write!" << std::endl;
            }
        } else {
            std::cout << "failed writing to pipe:" << result.error().message();
        }
    };

    std::cout << "send!" << std::endl;

    sendFunc(sendSpan);

    std::cout << "child: closed.." << std::endl;
}

void framingTestsParent(Pipe& pipeRead) {
    std::cout << "parent: Starting.." << std::endl;

    const int BUFFER_SIZE = 100;
    std::vector<std::byte> buffer(BUFFER_SIZE, std::byte{0});
    FrameReader frameReader;

    int nrOfReceivedFrames = 0;

    std::function<std::chrono::milliseconds()> getTimestamp = [](){
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
    };

    auto startTime = getTimestamp();

    bool done = false;
    while(!done) {
        absl::Span<std::byte> bufferSpan (buffer.data(), buffer.size());
        auto result = pipeRead.read(bufferSpan);
        if (!result) {
            std::cerr << fmt::format("server: failed reading from pipe with error: {0}!", strerror(errno)) << std::endl;
        }

        absl::Span<std::byte> readSpan (buffer.data(), result.value());
        for (auto frame = frameReader.read(readSpan) ; frame ;) {
            if (frame) {
                nrOfReceivedFrames++;
                std::cout << "Frame received!" << nrOfReceivedFrames << std::endl;
            }
            if (nrOfReceivedFrames == 2) {
                done = true;
                break;
            }

            auto timeoutElapsed = (getTimestamp() - startTime) > std::chrono::milliseconds(4000);
            ASSERT_EQ(timeoutElapsed, false); //timeout
        }


    }

    std::cout << "parent: closed.." << std::endl;
}

/***
 * - parent starts listening
 * - child sends message
 * - parent receives message
 */
TEST(KatlaIpcTests, IpcFramingTest) {
    
    Pipe pipeIn;
    Pipe pipeOut;

    outcome::result<void> result = pipeIn.open();
    ASSERT_TRUE(result) << result.error().message();

    result = pipeOut.open();
    if(!result) {
        FAIL() << result.error().message();
    }

    auto forkResult = fork();
    if (forkResult == -1) {
        ASSERT_NE(forkResult, -1);
    }

    if (forkResult == 0) {
        // child
        result = pipeIn.closeWrite();
        ASSERT_TRUE(result) << result.error().message();

        result = pipeOut.closeRead();
        ASSERT_TRUE(result) << result.error().message();

        framingTestsChild(pipeOut);

        result = pipeIn.close();
        ASSERT_TRUE(result) << result.error().message();

        result = pipeOut.close();
        ASSERT_TRUE(result) << result.error().message();

        exit(EXIT_SUCCESS);
    }

    // parent
    result = pipeIn.closeRead();
    ASSERT_TRUE(result) << result.error().message();

    result = pipeIn.closeWrite();
    ASSERT_TRUE(result) << result.error().message();

    framingTestsParent(pipeOut);

    result = pipeIn.close();
    ASSERT_TRUE(result) << result.error().message();

    result = pipeOut.close();
    ASSERT_TRUE(result) << result.error().message();

    exit(EXIT_SUCCESS);
}

}
