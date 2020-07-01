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

#include <chrono>
#include <mutex>
#include <utility>

namespace katla {

using namespace std::literals::chrono_literals;

WorkerThread::WorkerThread(std::string name, katla::PosixThread::Priority priority)
    : m_name(std::move(name)), m_priority(priority)
{}

WorkerThread::~WorkerThread() { join(); }

outcome::result<void, Error> WorkerThread::init(std::function<bool(void)> repeatableWork,
                                                std::chrono::milliseconds interval)
{
    m_interval = interval;
    m_thread = std::make_unique<std::thread>(&WorkerThread::exec, this, repeatableWork);

    return outcome::success();
}

void WorkerThread::wakeup() { m_condition.notify_all(); }

void WorkerThread::stop()
{
    m_stop = true;
    wakeup();
}

void WorkerThread::join()
{
    if (m_thread) {
        stop();
        m_thread->join();
        m_thread.reset();
    }
}

void WorkerThread::exec(const std::function<bool(void)>& repeatableWork)
{
    bool stop = false;
    bool noWait = false;

    auto setPriorityResult = katla::PosixThread::setPriority(*m_thread, m_priority);
    if (!setPriorityResult) {
        katla::printError("Failed setting thread priority: {}", setPriorityResult.error().message());
    }

    while (!stop) {
        {
            std::unique_lock<std::mutex> lock(m_mutex);

            if (!noWait) {
                m_condition.wait_for(lock, m_interval);
            }

            if (m_stop) {
                stop = true;
                continue;
            }
        }

        noWait = std::invoke(repeatableWork);
    }

    katla::print(stdout, "Closing thread {}!\n", m_name);
}

} // namespace katla