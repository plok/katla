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


#include "worker-thread.h"

#include "katla/core/core.h"
#include "katla/core/core-errors.h"

#include <chrono>
#include <mutex>
#include <utility>

namespace katla {

using namespace std::literals::chrono_literals;

WorkerThread::WorkerThread(std::string name, katla::Thread::Priority priority)
    : m_name(std::move(name)), m_priority(priority)
{}

WorkerThread::~WorkerThread() { join(); }

outcome::result<void, Error> WorkerThread::init(std::function<void(void)> repeatableWork,
                                                std::chrono::milliseconds interval)
{
    m_interval = interval;
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        if (m_thread) {
            return Error(katla::make_error_code(katla::CoreErrorCode::AlreadyInitialized));
        }

        m_stop = false;
        m_wakeUp = false;
        m_thread = std::make_unique<std::thread>(&WorkerThread::exec, this, repeatableWork);
    }

    return outcome::success();
}

void WorkerThread::wakeup() 
{
    {
        std::scoped_lock lock(m_mutex);
        m_wakeUp = true;
    }
    m_condition.notify_all(); 
}

void WorkerThread::stop()
{
    {
        std::scoped_lock lock(m_mutex);
        m_stop = true;
    }
    m_condition.notify_all();
}

bool WorkerThread::isStopped()
{
    std::scoped_lock lock(m_mutex);

    return m_stop;
}

void WorkerThread::join()
{
    if (m_thread) {
        m_thread->join();
        m_thread.reset();
    }
}

void WorkerThread::exec(const std::function<void(void)>& repeatableWork)
{
    {
        // need lock otherwise m_thread is not always assigned yet
        std::unique_lock<std::mutex> lock(m_mutex);

        auto setPriorityResult = katla::Thread::setPriority(*m_thread, m_priority);
        if (!setPriorityResult) {
            katla::printError("Failed setting thread priority: {}", setPriorityResult.error().message());
        }
    }

    while (true) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            m_condition.wait_for(lock, m_interval, [this]{ return m_wakeUp || m_stop; });
            if (m_stop) break;

            m_wakeUp = false;
        }

        std::invoke(repeatableWork);
    }
}

} // namespace katla
