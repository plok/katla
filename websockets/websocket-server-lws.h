
#ifndef KATLA_WEBSOCKET_LWS_H
#define KATLA_WEBSOCKET_LWS_H

#include "katla/core/core.h"
#include "katla/core/observable.h"
#include "katla/core/worker-thread.h"
#include "websocket-server-client-lws.h"

#include "http-request.h"

#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace katla {

struct WebSocketServerLwsPrivate;

class WebSocketServerLws {
  public:
    WebSocketServerLws();
    ~WebSocketServerLws();

    void init(int port);
    void stop();
    void join();

    void wakeup();

    void handleWebSocket(const std::string& url, const std::function<void(WebSocketServerClient&)>& callback);
    void handle(katla::HttpMethod method, const std::string& url, const std::function<void(WebSocketServerClient&, const HttpRequest&)>& callback);

  private:
    bool work();

    WebSocketServerLwsPrivate* d;
    WorkerThread m_workerThread;

    std::thread::id m_createThreadId;
    std::thread::id m_initThreadId;
    bool m_stopped { false };
};

} // namespace katla

#endif
