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
#include "uv-event-loop.h"

#include "katla/core/core.h"
#include "katla/core/core-errors.h"

#include "uv.h"

namespace katla {

UvEventLoop::UvEventLoop()
{
}

UvEventLoop::~UvEventLoop() {
    assert(!m_handle); // should be destroyed before destruction because it needs event-loop
}

outcome::result<void, Error> UvEventLoop::init()
{
    if (m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::AlreadyInitialized));
    }

    m_handle = new uv_loop_t();
    auto result = uv_loop_init(m_handle);
    if (result != 0) {
        return Error(katla::make_error_code(katla::CoreErrorCode::InitFailed), uv_strerror(result), uv_err_name(result));
    }

    return outcome::success();
}

outcome::result<void, Error> UvEventLoop::close()
{
    if (!m_handle) {
        return outcome::success();
    }

    // let handles be closed by event loop
    auto result = uv_run(m_handle, UV_RUN_NOWAIT);
    if (result != 0) {
        return Error(katla::make_error_code(katla::CoreErrorCode::CloseFailed), uv_strerror(result), uv_err_name(result));
    }

    result = uv_loop_close(m_handle);
    if (result != 0) {
        return Error(katla::make_error_code(katla::CoreErrorCode::CloseFailed), uv_strerror(result), uv_err_name(result));
    }

    // TODO check for busy and wait for a timeout period

    delete m_handle;
    m_handle = nullptr;

    return outcome::success();
}

outcome::result<void, Error> UvEventLoop::run()
{
    if (!m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::NotInitialized));
    }

    auto result = uv_run(m_handle, UV_RUN_DEFAULT);
    if (result != 0) {
        return Error(katla::make_error_code(katla::CoreErrorCode::OperationFailed), uv_strerror(result), uv_err_name(result));
    }

    return outcome::success();
}

outcome::result<void, Error> UvEventLoop::stop()
{
    if (!m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::NotInitialized));
    }

    uv_stop(m_handle);

    return outcome::success();
}

}
