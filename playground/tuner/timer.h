#ifndef TIMER_H
#define TIMER_H

#include "common/error.h"

#include <memory>
#include <functional>

class Timer {
public:
    virtual void init() = 0;

    // TODO use observable pattern instead
    virtual void start(int msec, std::function<void()> function) = 0;
    virtual void stop() = 0;

private:
    
};

#endif