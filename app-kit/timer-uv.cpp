#include "timer-uv.h"

#include <iostream>

UvTimer::UvTimer(const std::shared_ptr<UvEventLoop>& eventLoop)
{
    _eventLoop = eventLoop;
    _timer = new uv_timer_t();
    _timer->data = this;
}

UvTimer::~UvTimer() {
    stop();
    delete _timer;
}

void UvTimer::init()
{
    auto uvEventLoop = _eventLoop->uvEventLoop();
    auto result = uv_timer_init(uvEventLoop, _timer);
    if (result != 0) {
        // TODO
    }
}

void UvTimer::start(int msec, std::function<void()> function)
{
    _function = function;

    auto result = uv_timer_start(_timer, &uvTimerCallback, msec, msec);
    if (result != 0) {
        // TODO
    }
}

void UvTimer::stop()
{
    auto result = uv_timer_stop(_timer);
    if (result != 0) {
        // TODO
    }
}

void UvTimer::callback()
{
    // TODO check
    _function();
}

void UvTimer::uvTimerCallback(uv_timer_t* handle)
{
    static_cast<UvTimer*>(handle->data)->callback();
}