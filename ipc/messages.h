#ifndef KATLA_IPC_MESSAGES_H
#define KATLA_IPC_MESSAGES_H

#include "common/error.h"
#include <memory>
#include <variant>
#include <vector>

enum class FrameType {
    Unknown = 0,
    Initial,
    VersionHandshake,
    Settings,
    Header, // TODO Header + Data = Stream??
    Data
};

// TODO abstract to stream level?
enum class PayloadType {
    Custom,
    Control,
    Request,
    Push
};

struct FrameHeader {
    uint32_t frameLength;
    uint32_t frameNumber;
};

// Initial frame is used for first frame over connection from client to server
struct InitialHeader {
    FrameType type; // set to Initial for first frame
    int32_t version;
    
    int32_t source;
    int32_t destination;

    int32_t payloadOffset;
};

struct InitialFrame {
    FrameHeader frameHeader;
    InitialHeader header;

    PayloadType payloadType;
};

struct DataHeader {
    FrameType type;
    uint8_t spin; // for monitoring latency?

    int32_t destination;

    int32_t payloadOffset;
};

struct DataFrame {
    FrameHeader frameHeader;
    DataHeader header;

    PayloadType payloadType;
    // std::optional<PayloadPtr> payload;
};

struct CustomPayload {
    std::vector<std::byte> data;
};

struct Request {
    int32_t instanceId; // id of request instance
    int32_t id; // id of request
    int16_t format;
    std::vector<uint8_t> payload;
};

struct RequestResponse {
    int32_t instanceId; // id of request instance
    int32_t id; // id of request (how do we know the user doesn't require a string. So ID is internal?)
    int16_t format;
    std::vector<uint8_t> payload;
};

struct Push {
    int32_t id; // id of event / notification (how do we know the user doesn't require a string. So ID is internal?)
    int16_t format;
    std::vector<uint8_t> payload;
};

using PayloadPtr = std::variant<
    std::unique_ptr<CustomPayload>,
    std::unique_ptr<Request>,
    std::unique_ptr<RequestResponse>,
    std::unique_ptr<Push>>;

#endif
