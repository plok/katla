
#ifndef KATLA_WEBSOCKET_SERVER_CLIENT_H
#define KATLA_WEBSOCKET_SERVER_CLIENT_H

#include "http-request.h"

#include "katla/core/core.h"

#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace katla {

struct LwsPacket
{
    std::shared_ptr<std::vector<std::byte>> payload;
    bool isBinary {};
    bool isFirst {};
    bool isFinal {};
};

class WebSocketServerClient {
  public:
    virtual void send(const LwsPacket& message) = 0;

    virtual void registerMessageHandler(std::function<void(const LwsPacket&)> callback) = 0;
};

} // namespace katla

#endif
