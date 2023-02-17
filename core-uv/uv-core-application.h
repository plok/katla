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

#include "katla/core-uv/uv-event-loop.h"
#include "katla/core-uv/uv-signal-handler.h"
#include "katla/core/core-application.h"
#include "katla/core/core.h"
#include "katla/core/error.h"
#include "katla/core/event-loop.h"
#include "katla/core/signal-handler.h"
#include "katla/core/subject.h"
#include "katla/core/timer.h"

#include <functional>
#include <memory>
#include <mutex>
#include <atomic>

namespace katla {

class UvEventLoop;
class UvSignalHandler;

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

    static std::atomic<UvCoreApplication*> s_instancePtr;
    static std::mutex s_mutex;
    
    UvEventLoop m_eventLoop;
    UvSignalHandler m_interruptSignalHandler;
    UvSignalHandler m_terminateSignalHandler;
    UvSignalHandler m_hangupSignalHandler;
    UvSignalHandler m_childSignalHandler;

    Subject<void> m_onCloseSubject;
    Subject<void> m_onChildSubject;
};

} // namespace katla

#endif
