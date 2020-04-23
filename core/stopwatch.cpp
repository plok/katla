#include "stopwatch.h"
#include <chrono>

namespace katla
{

void Stopwatch::start (void)
{
    m_startTime = std::chrono::steady_clock::now();
}

long long Stopwatch::msecsElapsed (void)
{   
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - m_startTime).count();
}

long long Stopwatch::usecsElapsed (void)
{
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - m_startTime).count();
}

}
