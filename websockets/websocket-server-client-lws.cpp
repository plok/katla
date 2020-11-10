
#include "websocket-server-client-lws.h"

#include "websocket-server-client-lws-impl.h"

namespace katla {

WebSocketServerClientLws::WebSocketServerClientLws(WebSocketServerClientLwsImpl* d_) : d(d_) {}

WebSocketServerClientLws::~WebSocketServerClientLws() {}

void WebSocketServerClientLws::registerMessageHandler(std::function<void(const LwsPacket&)> callback)
{
    while (m_messageHandlers.find(++m_messageHandlerId) != m_messageHandlers.end()) {
        m_messageHandlerId++;
    }

    m_messageHandlers[m_messageHandlerId] = callback;
}

void WebSocketServerClientLws::handleMessage(const LwsPacket& message)
{
    for (auto& handler : m_messageHandlers) {
        handler.second(message);
    }
}
void WebSocketServerClientLws::send(const LwsPacket& message) { d->send(message); }

void WebSocketServerClientLws::sendHttpResult(const HttpRequestResult& result) { d->sendHttpResult(result); }

void WebSocketServerClientLws::handleDisconnect() {
    m_onDisconnectSubject.next();
}

} // namespace katla