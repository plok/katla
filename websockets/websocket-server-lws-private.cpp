#include "websocket-server-lws-private.h"

#include "incoming-http-request.h"

#include "websocket-server-client-lws-impl.h"
#include "websocket-server-client-lws.h"

#include <libwebsockets.h>
#include <variant>

namespace katla {

using namespace std::chrono_literals;

std::tuple<HttpMethod, std::string> WebSocketServerLwsPrivate::getMethod(lws* wsi)
{
    if (int length = lws_hdr_total_length(wsi, WSI_TOKEN_GET_URI); length > 0) {
        std::vector<char> uriVec(length + 1, 0);
        lws_hdr_copy(wsi, uriVec.data(), length + 1, WSI_TOKEN_GET_URI);
        return { HttpMethod::Get, std::string(uriVec.data()) };
    }
    if (int length = lws_hdr_total_length(wsi, WSI_TOKEN_POST_URI); length > 0) {
        std::vector<char> uriVec(length + 1, 0);
        lws_hdr_copy(wsi, uriVec.data(), length + 1, WSI_TOKEN_POST_URI);
        return { HttpMethod::Post, std::string(uriVec.data()) };
    }
    if (int length = lws_hdr_total_length(wsi, WSI_TOKEN_OPTIONS_URI); length > 0) {
        std::vector<char> uriVec(length + 1, 0);
        lws_hdr_copy(wsi, uriVec.data(), length + 1, WSI_TOKEN_OPTIONS_URI);
        return { HttpMethod::Options, std::string(uriVec.data()) };
    }

    return { HttpMethod::Unknown, 0 };
}

// void WebSocketServerLwsPrivate::addHttpClient(const std::shared_ptr<WebSocketServerClientLwsImpl>& client) {
//     httpClients.push_back(client);

//     for(auto& handlerList : httpHandlers) {
//         for (auto& handler : handlerList.second) {
//             handler(*client->m_publicClient);
//         }
//     }
// }

// void WebSocketServerLwsPrivate::removeHttpClient(const std::shared_ptr<WebSocketServerClientLwsImpl>& client) {
    
//     auto it = std::find(httpClients.begin(), httpClients.end(), client);
//     if (it != httpClients.end()) {
//         httpClients.erase(it);
//     }
// }

void WebSocketServerLwsPrivate::addWebSocketClient(const std::shared_ptr<WebSocketServerClientLwsImpl>& client) {
    webSocketClients.push_back(client);
}

void WebSocketServerLwsPrivate::removeWebSocketClient(const std::shared_ptr<WebSocketServerClientLwsImpl>& client) {

    auto it = std::find(webSocketClients.begin(), webSocketClients.end(), client);
    if (it != webSocketClients.end()) {
        webSocketClients.erase(it);
    }
}

void WebSocketServerLwsPrivate::handleHttpRequest(const std::shared_ptr<WebSocketServerClientLwsImpl>& client, const katla::HttpRequest& request) {
    
    for (auto& it : httpHandlers) {
        if (it.url != request.url || it.method != request.method) {
            continue;
        }

        for (auto& callbackIt : it.callbacks) {
            callbackIt(*client->m_publicClient, request);
        }
    }
}

} // namespace katla