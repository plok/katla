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

namespace katla {



void framingTestsChild(int fdIn, int fdOut) {
    std::cout << "child: Starting.." << std::endl;
    auto message = std::make_shared<InitialFrame>();
    message->header.type= FrameType::Initial;
    message->header.version= 0;

    absl::Span<std::byte> messageSpan (reinterpret_cast<std::byte*>(message.get()), sizeof(InitialFrame));

    std::function<void(absl::Span<std::byte>)> sendFunc = [fdOut](absl::Span<std::byte> frame) {
        std::cout << write(fdOut, frame.data(), frame.size()) << std::endl;
    };

//    std::function<void(std::shared_ptr<Message>)> receiveFunc = [&clientDispatcher](std::shared_ptr<Message> message) {
//        // TODO decode message

//        auto pongMessage = std::make_shared<Message>();

//    };


    std::cout << "send!" << std::endl;

    sendFunc(messageSpan);

    const int BUFFER_SIZE = 100;
    std::vector<std::byte> buffer(BUFFER_SIZE, std::byte{0});
    FrameReader frameReader;

    while(true) {
        // TODO use ringbuffer from frameReader
        ssize_t nbytes = read(fdIn, buffer.data(), buffer.size());
        if (nbytes == -1) {
            std::cerr << fmt::format("child: failed reading from pipe with error: {0}!", strerror(errno)) << std::endl;
        }

        absl::Span<std::byte> span (buffer.data(), nbytes);
        auto frame = frameReader.read(span);

//        auto receivedMessage = std::make_shared<Message>();


//        if (nbytes == 0) {
//            continue;
//        }

//        // TODO split stream in raw messages instead
//        if (nbytes != sizeof(Message)) {
//            std::cerr << fmt::format("child: received wrong message size: {0}!", nbytes) << std::endl;
//            exit(EXIT_FAILURE);
//        }
//        memcpy(receivedMessage.get(), buf, static_cast<size_t>(nbytes));

//        std::cout << fmt::format("child: received message of size: {0}!", nbytes) << std::endl;

//        clientDispatcher.handleMessage(receivedMessage);
    }

    close(fdIn);
    close(fdOut);

    std::cout << "child: closed.." << std::endl;

    exit(EXIT_SUCCESS);
}

void framingTestsParent(int fdIn, int fdOut) {
    std::cout << "parent: Starting.." << std::endl;

    std::function<void(absl::Span<std::byte>)> sendFunc = [fdOut](absl::Span<std::byte> message) {
        write(fdOut, message.data(), message.size());
    };

//    PipeMessageDispatcher clientDispatcher;
//    clientDispatcher.sendFunc = sendFunc;

    std::function<void(absl::Span<std::byte>)> receiveFunc = [](absl::Span<std::byte> message) {
//        std::cout << "receivedFromClient" << std::get<InitialFrame>(*message).length << "-" << (int)std::get<InitialFrame>(*message).type << "-" << std::endl;

//        auto pongMessage = std::make_shared<Message>();
//        pongMessage->type = MessageType::Custom;

//        CustomMessagePayload customPongPayload {};
//        customPongPayload.userType = 2;

//        std::string payloadPongString ("Pong!");
//        customPongPayload.payload.resize(payloadPongString.size());
//        memcpy(customPongPayload.payload.data(), payloadPongString.data(), customPongPayload.payload.size());

//        clientDispatcher.send(pongMessage);
    };

//    clientDispatcher.receiveFunc = receiveFunc;

    const int BUFFER_SIZE = 100;
    std::vector<std::byte> buffer(BUFFER_SIZE, std::byte{0});
    FrameReader frameReader;

    bool done = false;
    while(!done) {
        ssize_t nbytes = read(fdIn, buffer.data(), buffer.size());

        if (nbytes == -1) {
            std::cerr << fmt::format("server: failed reading from pipe with error: {0}!", strerror(errno)) << std::endl;
        }

        absl::Span<std::byte> span (buffer.data(), nbytes);
        auto frame = frameReader.read(span);

        //if (frame) {
        //    done = true;
        //}

//        auto receivedMessage = std::make_shared<Message>();

//        if (nbytes == 0) {
//            continue;
//        }

//        // TODO split stream in raw messages instead
//        if (nbytes != sizeof(Message)) {
//            std::cerr << fmt::format("server: received wrong message size: {0}!", nbytes) << std::endl;
//        }
//        memcpy(receivedMessage.get(), buf, static_cast<size_t>(nbytes));

//        std::cout << fmt::format("server: received message of size: {0}!", nbytes) << std::endl;

//        clientDispatcher.handleMessage(receivedMessage);
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
