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

#include "thread.h"

#ifdef WIN32
#include "katla/core/win32-thread.h"
#else
#include "katla/core/posix-thread.h"
#endif

namespace katla {

katla::result<void, Error> Thread::setPriority(std::thread& thread, Thread::Priority priority)
{
#ifdef WIN32
    return katla::Win32Thread::setPriority(thread, priority);
#else
    return katla::PosixThread::setPriority(thread, priority);
#endif
}

}