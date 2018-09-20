#ifndef TIMER_UV_H
#define TIMER_UV_H

#include "timer.h"
#include "common/error.h"

#include <memory>

#include <uv.h>

#include "event-loop-uv.h"

class UvTimer : public Timer {
public:
    UvTimer(UvEventLoop* eventLoop);
    virtual ~UvTimer();

    void init();
    void start(int msec, std::function<void()> function);
    void stop();

    void callback();
private:
    uv_loop_t* _eventLoop;
    uv_timer_t* _timer;

    std::function<void()> _function;
};

#endif