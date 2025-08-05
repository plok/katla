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

#include "katla/core/core-errors.h"
#include "katla/core/core.h"
#include "uv.h"

namespace katla {

UvEventLoop::UvEventLoop() {}

UvEventLoop::~UvEventLoop()
{
    assert(!m_handle); // should be destroyed before destruction because it needs event-loop
}

katla::result<void, Error> UvEventLoop::init()
{
    if (m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::AlreadyInitialized));
    }

    m_handle = new uv_loop_t();
    auto result = uv_loop_init(m_handle);
    if (result != 0) {
        return Error(
            katla::make_error_code(katla::CoreErrorCode::InitFailed), uv_strerror(result), uv_err_name(result));
    }

    return outcome::success();
}

katla::result<void, Error> UvEventLoop::close()
{
    if (!m_handle) {
        return outcome::success();
    }

    // let handles be closed by event loop
    auto runResult = runSingleIteration();
    if (!runResult) {
        return runResult.error();
    }

    auto result = uv_loop_close(m_handle);
    if (result != 0) {
        return Error(
            katla::make_error_code(katla::CoreErrorCode::CloseFailed), uv_strerror(result), uv_err_name(result));
    }

    // TODO check for busy and wait for a timeout period

    delete m_handle;
    m_handle = nullptr;

    return outcome::success();
}

katla::result<void, Error> UvEventLoop::run()
{
    if (!m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::NotInitialized));
    }

    auto result = uv_run(m_handle, UV_RUN_DEFAULT);
    if (result != 0) {
        return Error(
            katla::make_error_code(katla::CoreErrorCode::OperationFailed), katla::format("Error during uv_run: {}", uv_strerror(result)), uv_err_name(result));
    }

    return outcome::success();
}

katla::result<void, Error> UvEventLoop::runSingleIteration()
{
    if (!m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::NotInitialized));
    }

    auto result = uv_run(m_handle, UV_RUN_NOWAIT);
    if (result != 0) {
        return Error(
            katla::make_error_code(katla::CoreErrorCode::OperationFailed), katla::format("Error during uv_run: {}", uv_strerror(result)), uv_err_name(result));
    }

    return outcome::success();
}

katla::result<void, Error> UvEventLoop::stop()
{
    if (!m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::NotInitialized));
    }

    uv_stop(m_handle);

    return outcome::success();
}

katla::result<void, Error> UvEventLoop::printOpenHandles()
{
    if (!m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::NotInitialized));
    }

    uv_walk(m_handle, &printOpenHandlesUvWalkCallback, this);
    return outcome::success();
}

void UvEventLoop::printOpenHandlesUvWalkCallback(uv_handle_t* handle, void* /*arg*/)
{
    katla::printInfo("Open handle: {}", handle->type);
}

katla::result<void, Error> UvEventLoop::closeOpenHandles()
{
    if (!m_handle) {
        return Error(katla::make_error_code(katla::CoreErrorCode::NotInitialized));
    }

    m_handlesToClose.clear();
    uv_walk(m_handle, &closeOpenHandlesUvWalkCallback, this);

    while (!m_handlesToClose.empty()) {
        m_handlesToClose.clear();
        uv_walk(m_handle, &closeOpenHandlesUvWalkCallback, this);

        auto result = runSingleIteration();
        if (!result) {
            katla::printError("Failed running mosquitto iteration on close handles: {}", result.error().toString());
        }
    }

    return outcome::success();
}

void UvEventLoop::closeOpenHandlesUvWalkCallback(uv_handle_t* handle, void* arg)
{
    auto* eventLoop = reinterpret_cast<UvEventLoop*>(arg);
    eventLoop->m_handlesToClose.insert(handle);

    if (uv_is_closing(handle) != 0) {
        return;
    }

    katla::printInfo("Close handle: {}", handle->type);
    uv_close(handle, UvEventLoop::onCloseHandleCallback);
}

void UvEventLoop::onCloseHandleCallback(uv_handle_t* handle)
{
    katla::printInfo("Close handle type: {}", handle->type);
}

} // namespace katla
