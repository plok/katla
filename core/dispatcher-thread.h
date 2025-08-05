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
#include <atomic>

namespace katla {

class DispatcherThread {
  public:
    DispatcherThread(std::string name, katla::Thread::Priority priority);
    virtual ~DispatcherThread();

    katla::result<void, Error> init(std::chrono::milliseconds interval);

    static katla::result<std::reference_wrapper<DispatcherThread>, katla::Error> getDefault()
    {
        DispatcherThread* instance = m_instance.load(std::memory_order_consume);
        if (!instance) {
            std::scoped_lock lock(m_singletonMutex);
            instance = m_instance.load(std::memory_order_consume);
            if (!instance) {
                instance = new DispatcherThread("Default dispatcher", katla::Thread::Priority::Normal);
                
                auto initResult = instance->init(std::chrono::milliseconds(20));
                if (!initResult) {
                  return initResult.error();
                }

                m_instance.store(instance, std::memory_order_release);
            }
        }
        return std::ref(*instance);
    }

    void dispatch(const std::function<void()>& fn);

    void wakeup();

    void stop();
    void join();

  private:
    void repeatableWork(void);

    static std::mutex m_singletonMutex;
    static std::atomic<DispatcherThread*> m_instance;

    std::mutex m_mutex;    
    katla::WorkerThread _workerThread;

    std::queue<std::function<void()>> _execQueue;
};

} // namespace katla

#endif
