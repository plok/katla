
#ifndef KATLA_WEBSOCKET_SERVER_CLIENT_H
#define KATLA_WEBSOCKET_SERVER_CLIENT_H

#include "http-request.h"

#include "katla/core/core.h"
#include "katla/core/subject.h"

#include <chrono>
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace katla {

struct LwsPacket
{
    HttpStatusCode statusCode {};
    std::string contentType;
    std::map<std::string, std::string> headers;

    std::shared_ptr<std::vector<std::byte>> payload;
    int contentLength {};
    bool isBinary {};
    bool isFirst {};
    bool isFinal {};
};

class WebSocketServerClient {
  public:
    virtual void send(const LwsPacket& message) = 0;
    virtual void sendHttpResult(const HttpRequestResult& result) = 0;

    virtual void registerMessageHandler(std::function<void(const LwsPacket&)> callback) = 0;

    virtual std::unique_ptr<katla::Subscription> onDisconnect(std::function<void(void)> callback) = 0;
};

} // namespace katla

#endif
