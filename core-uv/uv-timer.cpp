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
#include "uv-timer.h"

#include "katla/core/error.h"
#include "katla/core/core-errors.h"

#include "uv.h"

namespace katla {

UvTimer::UvTimer(UvEventLoop& eventLoop) :
    m_eventLoop(eventLoop)
{
}

UvTimer::~UvTimer() {    
    assert(!m_handle); // should be destroyed before destruction because it needs event-loop
}

outcome::result<void, Error> UvTimer::init()
{
    if (m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::AlreadyInitialized));
    }

    m_handle = new uv_timer_t();
    m_handle->data = this;

    auto uvEventLoop = m_eventLoop.handle();
    auto result = uv_timer_init(uvEventLoop, m_handle);
    if (result != 0) {
        return Error(katla::make_error_code(katla::CoreErrorCode::InitFailed), uv_strerror(result), uv_err_name(result));
    }

    return outcome::success();
}

outcome::result<void, Error> UvTimer::close()
{
    if (!m_handle) {
        return outcome::success();
    }

    if (!uv_is_closing(reinterpret_cast<uv_handle_t*>(m_handle))) {
        uv_close(reinterpret_cast<uv_handle_t*>(m_handle), uv_close_callback);
    }

    return outcome::success();
}

outcome::result<void, Error> UvTimer::start(std::chrono::milliseconds msec, std::function<void()> function)
{
    if (!m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::NotInitialized));
    }

    m_function = function;

    auto result = uv_timer_start(
                m_handle,
                &uvTimerCallback,
                static_cast<uint64_t>(msec.count()),
                static_cast<uint64_t>(msec.count()));

    if (result != 0) {
        return Error(katla::make_error_code(katla::CoreErrorCode::StartFailed), uv_strerror(result), uv_err_name(result));
    }

    return outcome::success();
}

outcome::result<void, Error> UvTimer::stop()
{
    if (!m_handle) {
        return outcome::success();
    }

    auto result = uv_timer_stop(m_handle);
    if (result != 0) {
        return Error(katla::make_error_code(katla::CoreErrorCode::StopFailed), uv_strerror(result), uv_err_name(result));
    }

    return outcome::success();
}

void UvTimer::callback()
{
    if (m_function) {
        m_function();
    }
}

void UvTimer::uvTimerCallback(uv_timer_t* handle)
{
    if (handle->data) {
        static_cast<UvTimer*>(handle->data)->callback();
    }
}

void UvTimer::uv_close_callback(uv_handle_t* handle)
{
    assert(handle);
    assert(handle->data); // event-loop should close handle before destruction

    if (handle->data) {
        static_cast<UvTimer*>(handle->data)->deleteHandle();
    }
}

void UvTimer::deleteHandle()
{
    if (m_handle) {
        delete m_handle;
        m_handle = nullptr;
    }
}


}
