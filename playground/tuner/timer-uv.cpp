#include "timer-uv.h"

#include <iostream>

void uv_timer_callback(uv_timer_t* handle)
{
    static_cast<UvTimer*>(handle->data)->callback();
}

UvTimer::UvTimer(UvEventLoop* eventLoop)
{
    _eventLoop = eventLoop->uvEventLoop();
    _timer = new uv_timer_t();
    _timer->data = this;
}

UvTimer::~UvTimer() {
    delete _timer;
}

void UvTimer::init()
{
    auto result = uv_timer_init(_eventLoop, _timer);
    if (result != 0) {
        // TODO
    }
}

void UvTimer::start(int msec, std::function<void()> function)
{
    _function = function;

    auto result = uv_timer_start(_timer, &uv_timer_callback, msec, msec);
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