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


#include "dispatcher-thread.h"

#include "katla/core/core.h"
#include "katla/core/core-errors.h"

#include <chrono>
#include <mutex>
#include <utility>

namespace katla {

using namespace std::literals::chrono_literals;

DispatcherThread::DispatcherThread(std::string name, katla::Thread::Priority priority)
    : _workerThread(std::move(name), priority)
{}

DispatcherThread::~DispatcherThread() { join(); }

outcome::result<void, Error> DispatcherThread::init(std::chrono::milliseconds interval)
{
    return _workerThread.init([this]() {this->repeatableWork();}, interval);
}

void DispatcherThread::dispatch(const std::function<void()>& fn) {
    {
        std::unique_lock<std::mutex> lock(m_mutex);
        _execQueue.push(fn);
    }
    
    wakeup();
}

void DispatcherThread::wakeup() 
{
    _workerThread.wakeup();
}

void DispatcherThread::stop()
{
    _workerThread.stop();
}

void DispatcherThread::join()
{
    _workerThread.join();
}

void DispatcherThread::repeatableWork(void) {
    std::function<void()> task;
    {
        std::unique_lock<std::mutex> lock(m_mutex);

        if (_execQueue.empty()) {
            return;
        }
        
        task = std::move(_execQueue.front());
        _execQueue.pop();
    }

    if (task) {
        task();
    }
}


} // namespace katla
