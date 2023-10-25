/***
 * Copyright 2019 The Katla Authors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef KATLA_CORE_UV_CORE_APPLICATION_H
#define KATLA_CORE_UV_CORE_APPLICATION_H

#include "core/stopwatch.h"
#include "katla/core-uv/uv-event-loop.h"
#include "katla/core-uv/uv-signal-handler.h"
#include "katla/core/core-application.h"
#include "katla/core/core.h"
#include "katla/core/error.h"
#include "katla/core/event-loop.h"
#include "katla/core/signal-handler.h"
#include "katla/core/subject.h"
#include "katla/core/timer.h"

#include <deque>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <atomic>

namespace katla {

class UvEventLoop;
class UvSignalHandler;
class UvCoreApplication;

class Future {
  uint64_t id() { return m_id; }

protected:
  uint64_t m_id = {};
};

class UvFuture : public Future {
  friend class UvCoreApplication;
public:
  UvFuture(UvCoreApplication* owner, std::function<void()> callback) : m_owner(owner), m_callback(callback) {}
  UvFuture(const UvFuture&) = delete;
private:
  UvCoreApplication* m_owner = {};
  std::function<void()> m_callback;
  Stopwatch m_stopwatch;
};

class UvCoreApplication : public CoreApplication {
  public:
    UvCoreApplication(const UvCoreApplication&) = delete;
    ~UvCoreApplication() override;

    void operator=(const UvCoreApplication&) = delete;

    static UvCoreApplication& instance();
    static bool hasInstance();

    outcome::result<void, Error> init() override;
    outcome::result<void, Error> close() override;

    outcome::result<void, Error> run() override;
    outcome::result<void, Error> stop() override;

    outcome::result<std::unique_ptr<Timer>, Error> createTimer() override;

     __attribute__ ((warning ("Experimental code!"))) 
    outcome::result<std::shared_ptr<Future>, Error> invokeAsync(std::function<void()> callback);

    EventLoop& eventLoop() override;
    UvEventLoop& uvEventLoop();

    std::unique_ptr<Subscription> onClose(std::function<void(void)> closeCallback) override
    {
        auto observer = std::make_shared<FuncObserver<void>>(closeCallback);
        return m_onCloseSubject.subscribe(observer);
    }
    void clearOnCloseHandlers() override { m_onCloseSubject.clear(); }

    // TODO: not available on windows, only UV specific API for now
    std::unique_ptr<Subscription> onChild(std::function<void(void)> childCallback)
    {
        auto observer = std::make_shared<FuncObserver<void>>(childCallback);
        return m_onChildSubject.subscribe(observer);
    }
    void clearOnChildHandlers() { m_onChildSubject.clear(); }

  private:
    UvCoreApplication();
    static void uvAsyncCallback(uv_async_t* handle);
    static void uv_close_callback(uv_handle_t* handle);

    static std::atomic<UvCoreApplication*> s_instancePtr;
    static std::mutex s_mutex;
    
    UvEventLoop m_eventLoop;
    UvSignalHandler m_interruptSignalHandler;
    UvSignalHandler m_terminateSignalHandler;
    UvSignalHandler m_hangupSignalHandler;
    UvSignalHandler m_childSignalHandler;

    Subject<void> m_onCloseSubject;
    Subject<void> m_onChildSubject;

    uv_async_t m_asyncHandle = {};
    
    std::mutex m_futureMutex;
    std::atomic<uint64_t> m_lastFutureCounter;
    std::deque<std::shared_ptr<UvFuture>> m_tasks;
};

} // namespace katla

#endif
