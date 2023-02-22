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


#ifndef KATLA_WORKER_THREAD_H
#define KATLA_WORKER_THREAD_H

#include "katla/core/core.h"
#include "katla/core/thread.h"

#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>

namespace katla {

class WorkerThread {
  public:
    WorkerThread(std::string name, katla::Thread::Priority priority);
    virtual ~WorkerThread();

    expected<void, Error> init(std::function<void(void)> repeatableWork, std::chrono::milliseconds interval);

    void wakeup();

    void stop();
    void join();

  protected:
    bool m_skipWaitForNextCycle {false};

  private:
    void exec(const std::function<void(void)>& repeatableWork);

    std::unique_ptr<std::thread> m_thread;
    std::mutex m_mutex;
    std::condition_variable m_condition;

    std::string m_name { "WorkerThread" };

    katla::Thread::Priority m_priority { katla::Thread::Priority::Normal };

    bool m_stop {};
    bool m_wakeUp {};
    std::chrono::milliseconds m_interval { 1000 };
};

} // namespace katla

#endif
