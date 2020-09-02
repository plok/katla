
#ifndef KATLA_WEBSOCKET_LWS_PRIVATE_H
#define KATLA_WEBSOCKET_LWS_PRIVATE_H

#include "katla/core/core.h"
#include "katla/core/observable.h"
#include "katla/core/worker-thread.h"
#include "websocket-server-client-lws-impl.h"

#include "http-request.h"
#include "incoming-http-request.h"

#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace katla {

struct VhdWebSocketServer
{
    lws_context* context;
    lws_vhost* vhost;

    IncomingHttpRequest* request;
    std::shared_ptr<WebSocketServerClientLwsImpl> client;
};

struct WebSocketServerLwsPrivate
{
    static std::tuple<HttpMethod, std::string> getMethod(lws* wsi);

    // void addHttpClient(const std::shared_ptr<WebSocketServerClientLwsImpl>& client);
    // void removeHttpClient(const std::shared_ptr<WebSocketServerClientLwsImpl>& client);

    void addWebSocketClient(const std::shared_ptr<WebSocketServerClientLwsImpl>& client);
    void removeWebSocketClient(const std::shared_ptr<WebSocketServerClientLwsImpl>& client);

    // TODO use a promise for the bytes. So the body can be handles later
    void handleHttpRequest(const std::shared_ptr<WebSocketServerClientLwsImpl>& client, const katla::HttpRequest& request);

    const lws_protocol_vhost_options pvo = { nullptr, nullptr, "websocket-server", "" };

    lws_http_mount mount {};
    lws_context_creation_info info {};
    lws_context* context { nullptr };

    std::vector<lws_protocols> protocols;

    struct HttpHandlers {
      std::string url;
      HttpMethod method;
      std::vector<std::function<void(WebSocketServerClient&, const HttpRequest&)>> callbacks;
    };

    struct WebSocketHandlers {
      std::string url;
      std::vector<std::function<void(WebSocketServerClient&)>> callbacks;
    };

    std::vector<HttpHandlers> httpHandlers;
    std::vector<WebSocketHandlers> webSocketHandlers;

    std::vector<std::shared_ptr<WebSocketServerClientLwsImpl>> httpClients;
    std::vector<std::shared_ptr<WebSocketServerClientLwsImpl>> webSocketClients;
};



} // namespace katla

#endif
