#include "stopwatch.h"
#include <chrono>

#include "fmt/format.h"

#include <assert.h>

namespace katla
{

void Stopwatch::start (void)
{
    m_steadyStartTime = std::chrono::steady_clock::now();
    m_systemStartTime = std::chrono::system_clock::now();

    m_isStarted = true;
}

long long Stopwatch::msecsElapsed (void) const
{   
    assert(m_isStarted);

    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_steadyStartTime).count();
}

long long Stopwatch::usecsElapsed (void) const
{
    assert(m_isStarted);

    auto result = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_steadyStartTime).count();

    return result;
}

std::chrono::time_point<std::chrono::steady_clock> Stopwatch::steadyStartTime()
{
    return m_steadyStartTime;
}

std::chrono::time_point<std::chrono::system_clock> Stopwatch::systemStartTime()
{
    return m_systemStartTime;
}

}
