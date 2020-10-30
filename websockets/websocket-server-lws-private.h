
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



struct WebSocketServerLwsPrivate
{
    static std::tuple<HttpMethod, std::string> getMethod(lws* wsi);

    void addHttpClient(lws* wsi);
    void removeHttpClient(lws* wsi);

    void handleNewWebSocketClient(lws* wsi);
    void removeWebSocketClient(lws* wsi);

    // TODO use a promise for the bytes. So the body can be handles later
    void handleHttpRequest(const std::shared_ptr<WebSocketServerClientLwsImpl>& client, const katla::HttpRequest& request);

    static std::optional<http_status> toLwsStatusCode(HttpStatusCode statusCode);

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

    std::map<void*, std::shared_ptr<WebSocketServerClientLwsImpl>> httpClientsMap;
    std::map<void*, std::shared_ptr<WebSocketServerClientLwsImpl>> webSocketClientsMap;
};



} // namespace katla

#endif
