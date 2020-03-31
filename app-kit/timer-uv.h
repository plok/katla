#ifndef TIMER_UV_H
#define TIMER_UV_H

#include "katla-appkit.h"

#include "timer.h"
#include "common/error.h"

#include <memory>

#include <uv.h>

#include "event-loop-uv.h"

class KATLA_APPKIT_DECLSPEC UvTimer : public Timer {
public:
    UvTimer(const std::shared_ptr<UvEventLoop>& eventLoop);
    virtual ~UvTimer();

    void init();
    void start(int msec, std::function<void()> function);
    void stop();

    void callback();
private:
    static void uvTimerCallback(uv_timer_t* handle);

    std::shared_ptr<UvEventLoop> _eventLoop;

    uv_timer_t* _timer;

    std::function<void()> _function;
};

#endif