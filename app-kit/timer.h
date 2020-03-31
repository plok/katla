#ifndef TIMER_H
#define TIMER_H

#include "katla-appkit.h"

#include "common/error.h"

#include <memory>
#include <functional>

class KATLA_APPKIT_DECLSPEC Timer {
public:
    virtual void init() = 0;

    // TODO use observable pattern instead
    virtual void start(int msec, std::function<void()> function) = 0;
    virtual void stop() = 0;

private:
    
};

#endif