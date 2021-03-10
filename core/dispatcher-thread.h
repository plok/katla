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


#ifndef KATLA_DISPATCHER_THREAD_H
#define KATLA_DISPATCHER_THREAD_H

#include "katla/core/core.h"
#include "katla/core/thread.h"
#include "katla/core/worker-thread.h"

#include <chrono>
#include <condition_variable>
#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <future>
#include <map>
#include <queue>

namespace katla {

class DispatcherThread {
  public:
    DispatcherThread(std::string name, katla::Thread::Priority priority);
    virtual ~DispatcherThread();

    outcome::result<void, Error> init(std::chrono::milliseconds interval);

    static DispatcherThread& getDefault() {
        static DispatcherThread instance("Default dispatcher", katla::Thread::Priority::Normal);     
        instance.init(std::chrono::milliseconds(20));               
        return instance;
    }

    void dispatch(const std::function<void()>& fn);

    void wakeup();

    void stop();
    void join();

  private:
    void repeatableWork(void);

    std::mutex m_mutex;
    katla::WorkerThread _workerThread;

    std::queue<std::function<void()>> _execQueue;
};

} // namespace katla

#endif
