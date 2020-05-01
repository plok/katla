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

#include "katla/core/core.h"

#include "katla/core/error.h"
#include "katla/core/event-loop.h"
#include "katla/core/signal-handler.h"
#include "katla/core/timer.h"
#include "katla/core/core-application.h"

#include "katla/core-uv/uv-event-loop.h"
#include "katla/core-uv/uv-signal-handler.h"

#include <memory>

namespace katla {

class UvEventLoop;
class UvSignalHandler;

class UvCoreApplication : public CoreApplication {
public:
    UvCoreApplication();
    virtual ~UvCoreApplication() override;

    outcome::result<void, Error> init() override;
    outcome::result<void, Error> close() override;

    outcome::result<void, Error> run() override;
    outcome::result<void, Error> stop() override;

    outcome::result<std::unique_ptr<Timer>, Error> createTimer() override;

    EventLoop& eventLoop() override;
    UvEventLoop& uvEventLoop();
private:
    UvEventLoop m_eventLoop;
    UvSignalHandler m_interruptSignalHandler;
    UvSignalHandler m_terminateSignalHandler;
    UvSignalHandler m_hangupSignalHandler;
};

}

#endif
