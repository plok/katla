
#ifndef KATLA_WEBSOCKET_SERVER_CLIENT_LWS_IMPL_H
#define KATLA_WEBSOCKET_SERVER_CLIENT_LWS_IMPL_H

#include "katla/core/core.h"
#include "websocket-server-client.h"

#include <chrono>
#include <libwebsockets.h>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <list>
#include <mutex>

namespace katla {

class WebSocketServerClientLws;

class WebSocketServerClientLwsImpl {
  public:
    WebSocketServerClientLwsImpl(lws_context* serverContext, lws* wsi);
    ~WebSocketServerClientLwsImpl();

    //// used by lws server
    void insert(gsl::span<std::byte> bytes);
    virtual std::unique_ptr<LwsPacket> message();
    void handleMessage(const LwsPacket& message);
    LwsPacket dataToSend();
    bool hasDataToSend();
    //--

    void send(const LwsPacket& message);

    std::shared_ptr<WebSocketServerClientLws> m_publicClient;

    std::mutex m_mutex;
    std::list<LwsPacket> m_sendList;

    lws* m_wsi {nullptr};
    lws_context* m_serverContext {nullptr};

    std::unique_ptr<std::vector<std::byte>> m_receiveBuffer;
};

} // namespace katla

#endif
