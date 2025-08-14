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
#ifndef KATLA_CORE_UV_TIMER_H
#define KATLA_CORE_UV_TIMER_H

#include "katla/core/core.h"

#include "katla/core/timer.h"
#include "katla/core/error.h"

#include "uv-event-loop.h"

#include <memory>

#include <uv.h>

namespace katla {

class UvTimer : public Timer {
public:
    UvTimer(UvEventLoop& eventLoop);
    virtual ~UvTimer();

    katla::result<void, Error> init() override;
    [[maybe_unused]] katla::result<void, Error> close() override;
    bool isClosed() override;

    katla::result<void, Error> start(std::chrono::milliseconds msec, std::function<void()> function) override;
    [[maybe_unused]] katla::result<void, Error> stop() override;

private:
    static void uvTimerCallback(uv_timer_t* handle);
    static void uv_close_callback(uv_handle_t* handle);

    void deleteHandle();
    void callback();

    UvEventLoop& m_eventLoop;

    uv_timer_t* m_handle{nullptr};

    std::function<void()> m_function;
};

}

#endif
