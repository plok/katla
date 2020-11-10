
#ifndef KATLA_WEBSOCKET_SERVER_CLIENT_LWS_H
#define KATLA_WEBSOCKET_SERVER_CLIENT_LWS_H

#include "katla/core/core.h"
#include "katla/core/subject.h"
#include "websocket-server-client.h"

#include <chrono>
#include <libwebsockets.h>
#include <list>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace katla {

struct WebSocketServerClientLwsImpl;

class WebSocketServerClientLws : public WebSocketServerClient {
  public:
    explicit WebSocketServerClientLws(WebSocketServerClientLwsImpl* d);
    ~WebSocketServerClientLws();

    void send(const LwsPacket& message) override;
    void sendHttpResult(const HttpRequestResult& result) override;

    std::unique_ptr<katla::Subscription> onDisconnect(std::function<void(void)> callback)
    {
        auto observer = std::make_shared<katla::FuncObserver<void>>(callback);
        return m_onDisconnectSubject.subscribe(observer);
    }

    // TODO private??
    void registerMessageHandler(std::function<void(const LwsPacket&)> callback);
    void handleMessage(const LwsPacket& message);
    void handleDisconnect();

  private:
    WebSocketServerClientLwsImpl* d { nullptr };

    int m_messageHandlerId {};
    std::map<int, std::function<void(const LwsPacket&)>> m_messageHandlers;

    katla::Subject<void> m_onDisconnectSubject;
};

} // namespace katla

#endif
