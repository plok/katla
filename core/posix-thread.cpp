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

#include "posix-thread.h"

#include "posix-errors.h"

#include <pthread.h>

namespace katla {

expected<void, Error> PosixThread::setPriority(std::thread& thread, Thread::Priority priority)
{
#ifndef __APPLE__
    auto nativeHandle = thread.native_handle();

    int policy = SCHED_OTHER;

    sched_param param {};
    param.sched_priority = 0;

    switch(priority) {
        case Thread::Priority::Realtime: 
            policy = SCHED_RR;
            param.sched_priority = 10;
            break;
        case Thread::Priority::Normal:
            policy = SCHED_OTHER;
            break;
        case Thread::Priority::Idle:

            policy = SCHED_IDLE;
    }

    int result = pthread_setschedparam(nativeHandle, policy, &param);
    if (result != 0) {
        switch(result) {
            case ESRCH:
                return unexpected<Error>(make_error_code(PosixErrorCodes::NotFound), "Thread not found!");
            case EINVAL:
                return unexpected<Error>(make_error_code(PosixErrorCodes::Invalid), "Invalid policy");
            case EPERM:
                return unexpected<Error>(make_error_code(PosixErrorCodes::PermissionDenied), "Permission denied");
            case ENOTSUP:
            default: {
                return unexpected<Error>(make_error_code(PosixErrorCodes::OperationNotSupported), "Operation not supported");
            }
        }
    }

#else
    assert(false /*,"setPriority not implemented for macos"*/);
#endif

    return expected<void, Error>();
}

}