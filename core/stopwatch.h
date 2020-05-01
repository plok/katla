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
#ifndef KATLA_CORE_STOPWATCH_H_
#define KATLA_CORE_STOPWATCH_H_

#include <chrono>

namespace katla
{   
    class Stopwatch
    {
    public:
        virtual ~Stopwatch() = default;

        void start (void);

        long long msecsElapsed (void) const;
        long long usecsElapsed (void) const;

        std::chrono::time_point<std::chrono::steady_clock> steadyStartTime();
        std::chrono::time_point<std::chrono::system_clock> systemStartTime();

    private:
        bool m_isStarted {0};

        std::chrono::time_point<std::chrono::steady_clock> m_steadyStartTime {};
        std::chrono::time_point<std::chrono::system_clock> m_systemStartTime {};
    };
}

#endif
