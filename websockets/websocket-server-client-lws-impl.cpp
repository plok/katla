
#include "websocket-server-client-lws-impl.h"

#include "websocket-server-client-lws.h"
#include "websocket-server-lws-private.h"

#include "katla/core/core.h"

namespace katla {

const int PacketSize = 65550;

WebSocketServerClientLwsImpl::WebSocketServerClientLwsImpl(lws_context* serverContext, lws* wsi)
    : m_wsi(wsi), m_serverContext(serverContext)
{
    m_publicClient = std::make_shared<WebSocketServerClientLws>(this);

    auto [method, url] = WebSocketServerLwsPrivate::getMethod(wsi);
    m_method = method;
    m_url = url;

    katla::printInfo("Websocket client: {} - {}", (int)method, url);
}

WebSocketServerClientLwsImpl::~WebSocketServerClientLwsImpl() {}

void WebSocketServerClientLwsImpl::insert(gsl::span<std::byte> bytes)
{
    if (!m_receiveBuffer) {
        m_receiveBuffer = std::make_unique<std::vector<std::byte>>();
    }

    m_receiveBuffer->insert(m_receiveBuffer->end(), bytes.begin(), bytes.end());
}

std::unique_ptr<LwsPacket> WebSocketServerClientLwsImpl::message()
{
    auto message = std::make_unique<LwsPacket>();
    message->payload = std::move(m_receiveBuffer);

    m_receiveBuffer.reset();
    return message;
}

std::optional<LwsPacket> WebSocketServerClientLwsImpl::dataToSend()
{
    std::scoped_lock lock(m_mutex);

    if (m_sendList.empty()) {
        return {};
    }

    auto result = m_sendList.front();
    m_sendList.pop_front();
    return result;
}

bool WebSocketServerClientLwsImpl::hasDataToSend()
{
    std::scoped_lock lock(m_mutex);
    return !m_sendList.empty();
}

void WebSocketServerClientLwsImpl::send(const LwsPacket& message)
{
    // chop message in multiple packets if it's bigger then PacketSize
    if (message.payload->size() > PacketSize) {
        int idx = 0;
        while (idx < message.payload->size()) {
            LwsPacket sendMessage {};
            sendMessage.statusCode = message.statusCode;
            sendMessage.contentType = message.contentType;
            sendMessage.headers = message.headers;
            sendMessage.isFirst = idx == 0;

            int size = message.payload->size() - idx;
            if (size > PacketSize) {
                size = PacketSize;
            }

            // libwebsockets want's to have LWS_PRE bytes prepended for efficiency
            sendMessage.payload = std::make_unique<std::vector<std::byte>>(LWS_PRE + size);
            std::copy(
                message.payload->begin() + idx,
                message.payload->begin() + idx + size,
                sendMessage.payload->begin() + LWS_PRE);

            idx += PacketSize;
            sendMessage.isFinal = idx >= message.payload->size();
            sendMessage.isBinary = message.isBinary;

            {
                std::scoped_lock lock(m_mutex);
                m_sendList.push_back(sendMessage);
            }
        }
    } else {
        std::scoped_lock lock(m_mutex);

        LwsPacket sendMessage {};
        sendMessage.statusCode = message.statusCode;
        sendMessage.contentType = message.contentType;
        sendMessage.headers = message.headers;
        sendMessage.isFirst = true;
        sendMessage.isFinal = true;
        sendMessage.isBinary = message.isBinary;

        // libwebsockets want's to have LWS_PRE bytes prepended for efficiency
        sendMessage.payload = std::make_unique<std::vector<std::byte>>(LWS_PRE + message.payload->size());
        std::copy(
            message.payload->begin(),
            message.payload->end(),
            sendMessage.payload->begin() + LWS_PRE);

        m_sendList.push_back(sendMessage);
    }

    // Cancel poll so we can ask for writable callback
    lws_cancel_service(m_serverContext);
}

void WebSocketServerClientLwsImpl::sendHttpResult(const HttpRequestResult& result)
{
    katla::LwsPacket packet {};
    packet.statusCode = result.statusCode;
    packet.contentType = result.contentType;
    packet.headers = result.headers;
    packet.payload = std::make_shared<std::vector<std::byte>>(result.payload); // copy data
    packet.isBinary = true;

    send(packet);
}

void WebSocketServerClientLwsImpl::handleMessage(const LwsPacket& message)
{
    m_publicClient->handleMessage(message);
}

} // namespace katla