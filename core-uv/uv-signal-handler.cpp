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
#include "uv-signal-handler.h"

#include "katla/core/core-errors.h"
#include "uv-event-loop.h"

#include <csignal>
#include <signal.h>

namespace katla {

UvSignalHandler::UvSignalHandler(UvEventLoop& eventLoop) : m_eventLoop(eventLoop) {}

UvSignalHandler::~UvSignalHandler()
{
    assert(!m_handle); // should be destroyed before destruction because it needs event-loop
}

outcome::result<void, Error> UvSignalHandler::init()
{
    if (m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::AlreadyInitialized));
    }

    m_handle = new uv_signal_t();
    m_handle->data = this;

    auto uvEventLoop = m_eventLoop.handle();
    auto result = uv_signal_init(uvEventLoop, m_handle);
    if (result != 0) {
        return Error(
            katla::make_error_code(katla::CoreErrorCode::InitFailed), uv_strerror(result), uv_err_name(result));
    }

    return outcome::success();
}

outcome::result<void, Error> UvSignalHandler::close()
{
    if (!m_handle) {
        return outcome::success();
    }

    if (!uv_is_closing(reinterpret_cast<uv_handle_t*>(m_handle))) {
        uv_close(reinterpret_cast<uv_handle_t*>(m_handle), uv_close_callback);
    }

    return outcome::success();
}

outcome::result<void, Error> UvSignalHandler::start(Signal signal, std::function<void()> function)
{
    if (!m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::NotInitialized));
    }

    m_function = function;

    if (signal == Signal::Unknown) {
        return Error(katla::make_error_code(katla::CoreErrorCode::InvalidSignal));
    }

    int uvSignal = -1;
    switch (signal) {
    case Signal::Unknown:
        assert(false);
    case Signal::Child:
        uvSignal = SIGCHLD;
        break;
    case Signal::Interrupt:
        uvSignal = SIGINT;
        break;
    case Signal::Hangup:
        uvSignal = SIGHUP;
        break;
    case Signal::Kill:
        uvSignal = SIGKILL;
        break;
    case Signal::Stop:
        uvSignal = SIGSTOP;
        break;
    case Signal::Terminate:
        uvSignal = SIGTERM;
        break;
    }

    auto result = uv_signal_start(m_handle, &uv_signal_handler_callback, uvSignal);
    if (result != 0) {
        return Error(
            katla::make_error_code(katla::CoreErrorCode::OperationFailed), uv_strerror(result), uv_err_name(result));
    }

    return outcome::success();
}

outcome::result<void, Error> UvSignalHandler::stop()
{
    if (!m_handle) {
        return outcome::success();
    }

    if (uv_is_active(reinterpret_cast<uv_handle_t*>(m_handle))) {
        auto result = uv_signal_stop(m_handle);
        if (result != 0) {
            return Error(katla::make_error_code(katla::CoreErrorCode::OperationFailed),
                         uv_strerror(result),
                         uv_err_name(result));
        }
    }

    return outcome::success();
}

void UvSignalHandler::callback(int /*signum*/)
{
    if (m_function) {
        m_function();
    }
}

void UvSignalHandler::uv_signal_handler_callback(uv_signal_t* handle, int signum)
{
    if (handle->data) {
        static_cast<UvSignalHandler*>(handle->data)->callback(signum);
    }
}

void UvSignalHandler::uv_close_callback(uv_handle_t* handle)
{
    assert(handle);
    assert(handle->data); // event-loop should close handle before destruction

    if (handle->data) {
        static_cast<UvSignalHandler*>(handle->data)->deleteHandle();
    }
}

void UvSignalHandler::deleteHandle()
{
    if (m_handle) {
        delete m_handle;
        m_handle = nullptr;
    }
}

} // namespace katla
