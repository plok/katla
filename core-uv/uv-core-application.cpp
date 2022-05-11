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

#include <memory>

namespace katla {

UvCoreApplication::UvCoreApplication()
    : m_interruptSignalHandler(m_eventLoop), m_terminateSignalHandler(m_eventLoop), m_hangupSignalHandler(m_eventLoop), m_childSignalHandler(m_eventLoop)
{
}

UvCoreApplication::~UvCoreApplication() {
    auto _ = close();
};

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

    result = m_eventLoop.close();
    if (!result) {
        katla::print(stderr,
                     "Failed closing event-loop: {}\n    {}-{}\n",
                     result.error().message(),
                     result.error().description(),
                     result.error().info());
        return result.error();
    }

    return outcome::success();
}

outcome::result<std::unique_ptr<Timer>, Error> UvCoreApplication::createTimer()
{
    auto timer = std::make_unique<UvTimer>(m_eventLoop);

    auto result = timer->init();
    if (!result) {
        return result.error();
    }

    return std::move(timer);
}

EventLoop& UvCoreApplication::eventLoop() { return m_eventLoop; }
UvEventLoop& UvCoreApplication::uvEventLoop() { return m_eventLoop; }

} // namespace katla
