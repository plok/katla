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
#ifndef KATLA_CORE_UV_EVENT_LOOP_H
#define KATLA_CORE_UV_EVENT_LOOP_H

#include "katla/core/core.h"
#include "katla/core/error.h"
#include "katla/core/event-loop.h"

#include <memory>
#include <set>

#include <uv.h>

namespace katla {

class UvEventLoop : public EventLoop {
public:
    UvEventLoop();
    virtual ~UvEventLoop();

    katla::result<void, Error> init();
    [[maybe_unused]] katla::result<void, Error> close();

    katla::result<void, Error> run();
    katla::result<void, Error> runSingleIteration();

    katla::result<void, Error> stop();

    katla::result<void, Error> printOpenHandles();
    katla::result<void, Error> closeOpenHandles();

    uv_loop_t* handle() {
        return m_handle;
    }
private:
    static void printOpenHandlesUvWalkCallback(uv_handle_t *handle, void *arg);
    
    static void closeOpenHandlesUvWalkCallback(uv_handle_t *handle, void *arg);
    static void onCloseHandleCallback(uv_handle_t *handle);

    uv_loop_t* m_handle {nullptr};

    std::set<uv_handle_t*> m_handlesToClose;
};

}

#endif
