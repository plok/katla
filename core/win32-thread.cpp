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

#include "win32-thread.h"

#include "win32-errors.h"

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"

namespace katla {

outcome::result<void, Error> Win32Thread::setPriority(std::thread& thread, Thread::Priority priority)
{
    // Set the priority
    HANDLE win32Thread = GetCurrentThread();

    int winPriority = {};

    switch (priority)
    {
    case Thread::Priority::Idle: winPriority = THREAD_PRIORITY_IDLE; break;
    case Thread::Priority::Normal: winPriority = THREAD_PRIORITY_NORMAL; break;
    case Thread::Priority::Realtime: winPriority = THREAD_PRIORITY_TIME_CRITICAL; break;
    }

    auto setPriorityResult = SetThreadPriority (win32Thread, winPriority);
    if (!setPriorityResult) {
        auto lastError = GetLastError();

        char* msgbuf;
        FormatMessage (
            FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            lastError,
            0,
            (LPTSTR) &msgbuf,
            0, NULL);

        return Error(make_error_code(Win32ErrorCodes::OperationFailed), katla::format("Failed setting priority: {}", msgbuf));
    }

    return outcome::success();
}

}