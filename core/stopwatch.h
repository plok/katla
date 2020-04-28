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
