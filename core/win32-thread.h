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

#ifndef KATLA_WIN32_THREAD_H
#define KATLA_WIN32_THREAD_H

#include "katla/core/core.h"
#include "katla/core/error.h"

#include "katla/core/thread.h"

#include <gsl/span>

#include <optional>
#include <thread>

namespace katla {

class Win32Thread {
public:
    static outcome::result<void, Error> setPriority(std::thread& thread, Thread::Priority priority);
private:

};

}

#endif // KATLA_PIPE_H
