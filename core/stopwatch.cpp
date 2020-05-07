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

    m_isValid = true;
    m_isStarted = true;
}

void Stopwatch::stop()
{
    assert (m_isValid);
    assert (m_isStarted);

    m_steadyStopTime = std::chrono::steady_clock::now();
    m_systemStopTime = std::chrono::system_clock::now();

    m_isStarted = false;

    m_totalElapsed += m_steadyStopTime - m_steadyStartTime;
}

void Stopwatch::reset()
{
    m_totalElapsed = {};
    start();
}

int64_t Stopwatch::msecsElapsed (void) const
{   
    assert (m_isValid);

    auto stopTime = m_steadyStopTime;
    if (m_isStarted) {
        stopTime = std::chrono::steady_clock::now();
    }

    return std::chrono::duration_cast<std::chrono::milliseconds>(stopTime - m_steadyStartTime).count();
}

int64_t Stopwatch::usecsElapsed (void) const
{
    assert (m_isValid);

    auto stopTime = m_steadyStopTime;
    if (m_isStarted) {
        stopTime = std::chrono::steady_clock::now();
    }

    return std::chrono::duration_cast<std::chrono::microseconds>(stopTime - m_steadyStartTime).count();
}

int64_t Stopwatch::nsecsElapsed() const
{
    assert (m_isValid);

    auto stopTime = m_steadyStopTime;
    if (m_isStarted) {
        stopTime = std::chrono::steady_clock::now();
    }

    return std::chrono::duration_cast<std::chrono::nanoseconds>(stopTime - m_steadyStartTime).count();
}

Stopwatch::SteadyTimePoint Stopwatch::steadyStartTime()
{
    assert (m_isValid);

    return m_steadyStartTime;
}

Stopwatch::SystemTimePoint Stopwatch::systemStartTime()
{
    assert (m_isValid);

    return m_systemStartTime;
}

}
