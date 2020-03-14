#include "../ipc-kit.h"

#include "app-kit/core-application-uv.h"

#include "uv.h"

#include "../messages.h"

#include "gtest/gtest.h"

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

// TODO single threaded only
// TODO guard max items
// TODO timeout (clean map)
//class PipeMessageDispatcher {
//public:
//    PipeMessageDispatcher() :
//        _key(0)
//    {
//    }

//    void sendRequest(std::shared_ptr<Frame> message, std::function<void(std::shared_ptr<Frame>)> callback) {

//        // TODO use vector with free spots??
//        _key++;
//        while (_messages.find(_key) != std::end(_messages)) {
//            _key++;
//        }

////        if (message.get())
////        message.get->packetNumber = _key;

//        MessageInfo messageInfo = { message, callback};
//        _messages[_key] = messageInfo;

//        sendFunc(message);
//    }

//    void send(std::shared_ptr<Frame> message)
//    {
//        if (!sendFunc) {
//            throw std::runtime_error("No send function defined!");
//        }
//        sendFunc(message);
//    }

//    void handleMessage(std::shared_ptr<Frame> message) {
////        auto iter = _messages.find(message->messageQosId);
////        if (iter == _messages.end()) {
////            // unknown message
////            return;
////        }

//        // TODO for requests?
//        //iter->second.callback(message);

//        receiveFunc(message);
//    }

//    std::function<void(std::shared_ptr<Frame>)> receiveFunc;
//    std::function<void(std::shared_ptr<Frame>)> sendFunc;

//private:
//    struct MessageInfo {
//        std::shared_ptr<Frame> message;
//        std::function<void(std::shared_ptr<Frame>)> callback;
//    };

//    std::map<int, MessageInfo> _messages;
//    int _key;
//};

//void pipeMessageDispatcherChild(int fdIn, int fdOut) {
//    std::cout << "child: Starting.." << std::endl;
//    auto message = std::make_shared<Frame>();
//    message->header = InitialHeader {
//                                                 .type= FrameType::Initial,
//                                                 .version= 0
//                                              };

////    CustomMessagePayload customPayload {};
////    customPayload.userType = 1;

////    std::string payloadString ("Hello world!");
////    customPayload.payload.resize(payloadString.size());
////    memcpy(customPayload.payload.data(), payloadString.data(), customPayload.payload.size());

//    std::function<void(std::shared_ptr<Frame>)> sendFunc = [fdOut](std::shared_ptr<Frame> message) {
//        write(fdOut, message.get(), sizeof(Frame));
//    };

//    PipeMessageDispatcher clientDispatcher;
//    clientDispatcher.sendFunc = sendFunc;

//    std::function<void(std::shared_ptr<Frame>)> receiveFunc = [&clientDispatcher](std::shared_ptr<Frame> message) {
//        // TODO decode message

//        auto pongMessage = std::make_shared<Frame>();

////        CustomMessagePayload customPongPayload {};
////        customPongPayload.userType = 2;

////        std::string payloadPongString ("Pong!");
////        customPongPayload.payload.resize(payloadPongString.size());
////        memcpy(customPongPayload.payload.data(), payloadPongString.data(), customPongPayload.payload.size());

//        clientDispatcher.send(pongMessage);
//    };

//    clientDispatcher.receiveFunc = receiveFunc;

//    std::cout << "send!" << std::endl;
//    clientDispatcher.send(message);

//    const int BUFFER_SIZE = 100;
//    std::vector<uint8_t> buffer(BUFFER_SIZE, 0);

//    while(true) {
//        // TODO use ringbuffer from frameReader
//        ssize_t nbytes = read(fdIn, buffer.data(), buffer.size());
//        if (nbytes == -1) {
//            std::cerr << fmt::format("child: failed reading from pipe with error: {0}!", strerror(errno)) << std::endl;
//        }

////        auto receivedMessage = std::make_shared<Message>();


////        if (nbytes == 0) {
////            continue;
////        }

////        // TODO split stream in raw messages instead
////        if (nbytes != sizeof(Message)) {
////            std::cerr << fmt::format("child: received wrong message size: {0}!", nbytes) << std::endl;
////            exit(EXIT_FAILURE);
////        }
////        memcpy(receivedMessage.get(), buf, static_cast<size_t>(nbytes));

////        std::cout << fmt::format("child: received message of size: {0}!", nbytes) << std::endl;

////        clientDispatcher.handleMessage(receivedMessage);
//    }

//    close(fdIn);
//    close(fdOut);

//    exit(EXIT_SUCCESS);
//}

//void pipeMessageDispatcherParent(int fdIn, int fdOut) {
//    std::cout << "parent: Starting.." << std::endl;

//    std::function<void(std::shared_ptr<Frame>)> sendFunc = [fdOut](std::shared_ptr<Frame> message) {
//        write(fdOut, message.get(), sizeof(Frame));
//    };

//    PipeMessageDispatcher clientDispatcher;
//    clientDispatcher.sendFunc = sendFunc;

//    std::function<void(std::shared_ptr<Frame>)> receiveFunc = [&clientDispatcher](std::shared_ptr<Frame> message) {
//       // std::cout << "receivedFromClient" << std::get<InitialFrame>(*message).version << "-" << (int)std::get<InitialFrame>(*message).type << "-" << std::endl;

////        auto pongMessage = std::make_shared<Message>();
////        pongMessage->type = MessageType::Custom;

////        CustomMessagePayload customPongPayload {};
////        customPongPayload.userType = 2;

////        std::string payloadPongString ("Pong!");
////        customPongPayload.payload.resize(payloadPongString.size());
////        memcpy(customPongPayload.payload.data(), payloadPongString.data(), customPongPayload.payload.size());

////        clientDispatcher.send(pongMessage);
//    };

//    clientDispatcher.receiveFunc = receiveFunc;

//    const int BUFFER_SIZE = 100;
//    std::vector<uint8_t> buffer(BUFFER_SIZE, 0);

//    while(true) {
//        ssize_t nbytes = read(fdIn, buffer.data(), buffer.size());

//        if (nbytes == -1) {
//            std::cerr << fmt::format("server: failed reading from pipe with error: {0}!", strerror(errno)) << std::endl;
//        }

////        auto receivedMessage = std::make_shared<Message>();

////        if (nbytes == 0) {
////            continue;
////        }

////        // TODO split stream in raw messages instead
////        if (nbytes != sizeof(Message)) {
////            std::cerr << fmt::format("server: received wrong message size: {0}!", nbytes) << std::endl;
////        }
////        memcpy(receivedMessage.get(), buf, static_cast<size_t>(nbytes));

////        std::cout << fmt::format("server: received message of size: {0}!", nbytes) << std::endl;

////        clientDispatcher.handleMessage(receivedMessage);
//    }
//    close(fdIn);
//    close(fdOut);
//}


//TEST(KatlaIpcTests, PosixPipeDispatcher) {

//    // TODO pipe class
//    int pipeInFd[2];
//    auto pipeInStatus = pipe(pipeInFd);
//    if (pipeInStatus == -1) {
//        auto pipeInErrno = errno;
//        EXPECT_NE(pipeInErrno, 0);
//        ASSERT_NE(pipeInStatus, -1);
//    }

//    int pipeOutFd[2];
//    auto pipeOutStatus = pipe(pipeOutFd);
//    if (pipeOutStatus == -1) {
//        auto pipeOutErrno = errno;
//        EXPECT_NE(pipeOutErrno, 0);
//        ASSERT_NE(pipeOutStatus, -1);
//    }

//    auto forkResult = fork();
//    if (forkResult == -1) {
//        ASSERT_NE(forkResult, -1);
//    }

//    if (forkResult == 0) {
//        // child
//        close(pipeInFd[1]);
//        close(pipeOutFd[0]);
//        pipeMessageDispatcherChild(pipeInFd[0], pipeOutFd[1]);

//        exit(EXIT_SUCCESS);
//    }

//    // parent
//    close(pipeInFd[0]);
//    close(pipeOutFd[1]);
//    pipeMessageDispatcherParent(pipeOutFd[0], pipeInFd[1]);

//}

}
