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
#ifndef TIMER_H
#define TIMER_H

#include "katla/core/core.h"
#include "katla/core/error.h"

#include <chrono>
#include <functional>

namespace katla {

class Timer {
public:
    virtual ~Timer() = default;

    virtual outcome::result<void, Error> init() = 0;
    virtual outcome::result<void, Error> close() = 0;
    virtual bool isClosed() = 0;
    
    virtual outcome::result<void, Error> start(std::chrono::milliseconds msec, std::function<void()> function) = 0;
    virtual outcome::result<void, Error> stop() = 0;
private:
    
};

 }

#endif
