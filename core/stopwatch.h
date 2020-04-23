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

        long long msecsElapsed (void);
        long long usecsElapsed (void);

    private:
        std::chrono::time_point<std::chrono::steady_clock> m_startTime;
    };
}

#endif
