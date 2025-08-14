
#ifndef KATLA_WEBSOCKET_SERVER_CLIENT_LWS_IMPL_H
#define KATLA_WEBSOCKET_SERVER_CLIENT_LWS_IMPL_H

#include "katla/core/core.h"
#include "websocket-server-client.h"
#include "katla/websockets/incoming-http-request.h"

#define WIN32_LEAN_AND_MEAN
#include "libwebsockets.h"

#include <chrono>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <mutex>

namespace katla {

class WebSocketServerClientLws;

// TODO can be removed?
struct VhdWebSocketServer
{
    lws_context* context;
    lws_vhost* vhost;
};

class WebSocketServerClientLwsImpl {
  public:
    WebSocketServerClientLwsImpl(lws_context* serverContext, lws* wsi);
    ~WebSocketServerClientLwsImpl();

    //// used by lws server
    void insert(katla::span<std::byte> bytes);
    virtual std::unique_ptr<LwsPacket> message();
    void handleMessage(const LwsPacket& message);
    std::optional<LwsPacket> dataToSend();
    bool hasDataToSend();

    void handleDisconnect();
    //--

    void send(const LwsPacket& message);
    void sendHttpResult(const HttpRequestResult& result);

    std::shared_ptr<WebSocketServerClientLws> m_publicClient;

    std::mutex m_mutex;
    std::list<LwsPacket> m_sendList;

    lws* m_wsi {nullptr};
    lws_context* m_serverContext {nullptr};

    std::unique_ptr<std::vector<std::byte>> m_receiveBuffer;

    std::string m_url;
    HttpMethod m_method;

    std::shared_ptr<IncomingHttpRequest> m_request;
};

} // namespace katla

#endif
