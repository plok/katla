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
        using SteadyTimePoint = std::chrono::time_point<std::chrono::steady_clock>;
        using SystemTimePoint = std::chrono::time_point<std::chrono::system_clock>;

    public:
        virtual ~Stopwatch() = default;

        void start (void);
        void stop (void);

        void reset (void);

        std::chrono::steady_clock::duration elapsed() const;
        int64_t msecsElapsed (void) const;
        int64_t usecsElapsed (void) const;
        int64_t nsecsElapsed (void) const;

        SteadyTimePoint steadyStartTime();
        SystemTimePoint systemStartTime();

        bool isValid() const {return m_isValid;}
        bool isStarted() const {return m_isStarted;}

    private:
        bool m_isValid {false};
        bool m_isStarted {false};

        SteadyTimePoint m_steadyStartTime {};
        SystemTimePoint m_systemStartTime {};

        SteadyTimePoint m_steadyStopTime {};
        SystemTimePoint m_systemStopTime {};

        std::chrono::nanoseconds m_totalElapsed {};
    };
}

#endif
