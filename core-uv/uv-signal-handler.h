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
#ifndef KATLA_CORE_UV_SIGNAL_HANDLER_H
#define KATLA_CORE_UV_SIGNAL_HANDLER_H

#include "katla/core/core.h"
#include "katla/core/signal-handler.h"
#include "katla/core/error.h"

#include <memory>
#include <uv.h>

namespace katla {

class UvEventLoop;

class UvSignalHandler : public SignalHandler {
public:
    UvSignalHandler(UvEventLoop& eventLoop);
    virtual ~UvSignalHandler();

    outcome::result<void, Error> init();
    outcome::result<void, Error> close();

    outcome::result<void, Error> start(Signal signal, std::function<void()> callback);
    outcome::result<void, Error> stop();

private:
    static void uv_signal_handler_callback(uv_signal_t* handle, int signum);
    static void uv_close_callback(uv_handle_t* handle);

    void deleteHandle();

    // use by callback only
    void callback(int signum);
    void setClosed(bool closed) {m_closed = closed;}

    UvEventLoop& m_eventLoop;

    uv_signal_t* m_handle {nullptr};
    std::function<void()> m_function;

    bool m_closed {true};
};

}

#endif
