#include "../ipc-kit.h"

#include "app-kit/core-application-uv.h"

#include "uv.h"

#include "../messages.h"
#include "../frame-reader.h"

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
void framingTestsChild(int fdIn, int fdOut) {
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

    std::function<void(absl::Span<std::byte>)> sendFunc = [fdOut](absl::Span<std::byte> frame) {
        std::cout << write(fdOut, frame.data(), frame.size()) << std::endl;
    };

    std::cout << "send!" << std::endl;

    sendFunc(sendSpan);

    close(fdIn);
    close(fdOut);

    std::cout << "child: closed.." << std::endl;

    exit(EXIT_SUCCESS);
}

void framingTestsParent(int fdIn, int fdOut) {
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
        ssize_t nbytes = read(fdIn, buffer.data(), buffer.size());

        if (nbytes == -1) {
            std::cerr << fmt::format("server: failed reading from pipe with error: {0}!", strerror(errno)) << std::endl;
        }

        absl::Span<std::byte> span (buffer.data(), nbytes);
        for (auto frame = adsf frameReader.read(span) ; frame ;) {
            if (frame) {
                nrOfReceivedFrames++;
                std::cout << "Frame received!" << nrOfReceivedFrames << std::endl;
            }
            if (nrOfReceivedFrames == 2) {
                done = true;
            }

            auto timeoutElapsed = (getTimestamp() - startTime) > std::chrono::milliseconds(4000);
            ASSERT_EQ(timeoutElapsed, false); //timeout
        }


    }

    close(fdIn);
    close(fdOut);

    std::cout << "parent: closed.." << std::endl;
}

/***
 * - parent starts listening
 * - child sends message
 * - parent receives message
 */
TEST(KatlaIpcTests, IpcFramingTest) {
    
    // TODO pipe class
    int pipeInFd[2];
    auto pipeInStatus = pipe(pipeInFd);
    if (pipeInStatus == -1) {
        auto pipeInErrno = errno;
        EXPECT_NE(pipeInErrno, 0);
        ASSERT_NE(pipeInStatus, -1);
    }

    int pipeOutFd[2];
    auto pipeOutStatus = pipe(pipeOutFd);
    if (pipeOutStatus == -1) {
        auto pipeOutErrno = errno;
        EXPECT_NE(pipeOutErrno, 0);
        ASSERT_NE(pipeOutStatus, -1);
    }

    auto forkResult = fork();
    if (forkResult == -1) {
        ASSERT_NE(forkResult, -1);
    }

    if (forkResult == 0) {
        // child
        close(pipeInFd[1]);
        close(pipeOutFd[0]);
        framingTestsChild(pipeInFd[0], pipeOutFd[1]);

        exit(EXIT_SUCCESS);
    }

    // parent
    close(pipeInFd[0]);
    close(pipeOutFd[1]);
    framingTestsParent(pipeOutFd[0], pipeInFd[1]);

}

}
