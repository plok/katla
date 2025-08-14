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

#ifndef KATLA_CORE_APPLICATION_H
#define KATLA_CORE_APPLICATION_H

#include "katla/core/core.h"
#include "katla/core/error.h"
#include "katla/core/event-loop.h"
#include "katla/core/observable.h"
#include "katla/core/signal-handler.h"
#include "katla/core/timer.h"

#include <memory>

namespace katla {

class CoreApplication {
public:
    CoreApplication() = default;
    virtual ~CoreApplication() = default;

    virtual katla::result<void, Error> init() = 0;
    virtual katla::result<void, Error> close() = 0;

    virtual katla::result<void, Error> run() = 0;
    virtual katla::result<void, Error> stop() = 0;

    virtual katla::result<std::unique_ptr<Timer>, Error> createTimer() = 0;

    virtual EventLoop& eventLoop() = 0;

    virtual std::unique_ptr<Subscription> onClose(std::function<void(void)> closeCallback) = 0;
    virtual void clearOnCloseHandlers() = 0;
};

}

#endif
