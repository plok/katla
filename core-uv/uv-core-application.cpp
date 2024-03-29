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
#include "uv-core-application.h"

#include "uv-event-loop.h"
#include "uv-signal-handler.h"
#include "uv-timer.h"
#include "core/core.h"
#include "core/core-errors.h"

#include <memory>
#include <gsl/gsl>

namespace katla {

std::atomic<UvCoreApplication*> UvCoreApplication::s_instancePtr;
std::mutex UvCoreApplication::s_mutex;

UvCoreApplication::UvCoreApplication()
    : m_interruptSignalHandler(m_eventLoop), m_terminateSignalHandler(m_eventLoop), m_hangupSignalHandler(m_eventLoop), m_childSignalHandler(m_eventLoop)
{
}

UvCoreApplication::~UvCoreApplication() {
    auto _ = close();
};

UvCoreApplication& UvCoreApplication::instance()
{
    static UvCoreApplication coreApp;

    // double locking might not be needed for just the pointer, but adding it for now just to be safe
    auto* p = s_instancePtr.load(std::memory_order_acquire);
    if (p == nullptr) {
        std::lock_guard lock(s_mutex);
        p = s_instancePtr.load(std::memory_order_relaxed);
        if (p == nullptr) {
            p = &coreApp;
            s_instancePtr.store(p, std::memory_order_release);
        }
    }
    assert(p != nullptr);
    return coreApp;
}

bool UvCoreApplication::hasInstance()
{
    UvCoreApplication* p = s_instancePtr.load(std::memory_order_acquire);
    return (p != nullptr);
}

outcome::result<void, Error> UvCoreApplication::init()
{
    auto error = m_eventLoop.init();
    if (!error) {
        return error;
    }

    error = m_interruptSignalHandler.init();
    if (!error) {
        return error;
    }
    error = m_terminateSignalHandler.init();
    if (!error) {
        return error;
    }
    error = m_hangupSignalHandler.init();
    if (!error) {
        return error;
    }
    error = m_childSignalHandler.init();
    if (!error) {
        return error;
    }

    m_asyncHandle.data = this;
    int uvError = uv_async_init(m_eventLoop.handle(), &m_asyncHandle, &UvCoreApplication::uvAsyncCallback);
    if (uvError != 0) {
        return Error(katla::make_error_code(katla::CoreErrorCode::OperationFailed),
                         katla::format("Error during uv_async_init: {} {}", uvError, uv_strerror(uvError)),
                         uv_err_name(uvError));
    }

    error = m_interruptSignalHandler.start(Signal::Interrupt, [this]() {
        katla::print(stdout, "Interrupt signal received\n");
        m_onCloseSubject.next();
    });
    if (!error) {
        return error;
    }

    error = m_terminateSignalHandler.start(Signal::Terminate, [this]() {
        katla::print(stdout, "Terminate signal received\n");
        m_onCloseSubject.next();
    });
    if (!error) {
        return error;
    }

    error = m_hangupSignalHandler.start(Signal::Hangup, [this]() {
        katla::print(stdout, "Hangup signal received\n");
        m_onCloseSubject.next();
    });
    if (!error) {
        return error;
    }

    error = m_childSignalHandler.start(Signal::Child, [this]() {
        katla::print(stdout, "Child signal received\n");
        m_onChildSubject.next();
    });
    if (!error) {
        return error;
    }

    return outcome::success();
}

outcome::result<void, Error> UvCoreApplication::run() { return m_eventLoop.run(); }

outcome::result<void, Error> UvCoreApplication::stop()
{
    auto result = m_interruptSignalHandler.stop();
    if (!result) {
        return result.error();
    }

    result = m_terminateSignalHandler.stop();
    if (!result) {
        return result.error();
    }

    result = m_hangupSignalHandler.stop();
    if (!result) {
        return result.error();
    }

    result = m_childSignalHandler.stop();
    if (!result) {
        return result.error();
    }

    if (!uv_is_closing(reinterpret_cast<uv_handle_t*>(&m_asyncHandle))) {
        uv_close(reinterpret_cast<uv_handle_t*>(&m_asyncHandle), uv_close_callback);
    }

    // At this point the eventloop should automatically close unless there are still handles open
    // do a manual stop to make sure the eventloop stops so we can call the close() method outside the eventloop.
    result = m_eventLoop.stop();
    if (!result) {
        return result.error();
    }

    return outcome::success();
}

outcome::result<void, Error> UvCoreApplication::close()
{
    auto result = m_interruptSignalHandler.close();
    if (!result) {
        katla::print(stderr, result.error().message());
    }
    result = m_terminateSignalHandler.close();
    if (!result) {
        katla::print(stderr, result.error().message());
    }
    result = m_hangupSignalHandler.close();
    if (!result) {
        katla::print(stderr, result.error().message());
    }
    result = m_childSignalHandler.close();
    if (!result) {
        katla::print(stderr, result.error().message());
    }

    if (!uv_is_closing(reinterpret_cast<uv_handle_t*>(&m_asyncHandle))) {
        uv_close(reinterpret_cast<uv_handle_t*>(&m_asyncHandle), uv_close_callback);
    }

    if (m_eventLoop.handle()) {
        result = m_eventLoop.runSingleIteration();
        if (!result) {
            katla::printError("Failed running eventloop iteration on close: {}", result.error().toString());
        }

        result = m_eventLoop.closeOpenHandles();
        if (!result) {
            katla::printError("Failed closing open handles: {}", result.error().toString());
        }

        result = m_eventLoop.close();
        if (!result) {
            katla::printError("Failed closing event-loop: {}", result.error().toString());
            return result.error();
        }
    }

    return outcome::success();
}

void UvCoreApplication::uv_close_callback(uv_handle_t* handle)
{
    assert(handle);
    assert(handle->data); // event-loop should close handle before destruction
}

outcome::result<std::unique_ptr<Timer>, Error> UvCoreApplication::createTimer()
{
    auto timer = std::make_unique<UvTimer>(m_eventLoop);

    auto result = timer->init();
    if (!result) {
        return result.error();
    }

    return timer;
}

void UvCoreApplication::uvAsyncCallback(uv_async_t* handle)
{
    auto* app = reinterpret_cast<UvCoreApplication*>(handle->data);

    bool empty = false;
    while(!empty) 
    {
        std::shared_ptr<UvFuture> future;
        {
            std::scoped_lock lock(app->m_futureMutex);
            empty = app->m_tasks.empty();
            if (empty) {
                continue;
            }

            future = app->m_tasks.front();
            app->m_tasks.pop_front();
        }
        
        if (!future) {
            continue;
        }

        future->m_callback.operator()();

        // katla::printInfo("Elapsed async method: {} {}", future->id, future->stopwatch.usecsElapsed());
    }
};

outcome::result<std::shared_ptr<Future>, Error> UvCoreApplication::invokeAsync(std::function<void()> callback)
{
    auto result = std::make_shared<UvFuture>(this, callback);

    {
        std::scoped_lock lock(m_futureMutex);
        result->m_id = ++m_lastFutureCounter;
        m_tasks.push_back(result);
    }

    result->m_stopwatch.start();
    auto sendResult = uv_async_send(&m_asyncHandle);
    if (sendResult != 0) {
        return Error(katla::make_error_code(katla::CoreErrorCode::OperationFailed), katla::format("Error during uv_async_send: {}", uv_strerror(sendResult)), uv_err_name(sendResult));
    }

    return result;
}

EventLoop& UvCoreApplication::eventLoop() { return m_eventLoop; }
UvEventLoop& UvCoreApplication::uvEventLoop() { return m_eventLoop; }

} // namespace katla
