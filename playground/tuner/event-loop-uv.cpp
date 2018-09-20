#include "event-loop-uv.h"

UvEventLoop::UvEventLoop()
{
    _eventLoop = new uv_loop_t();
}

UvEventLoop::~UvEventLoop() {
    delete _eventLoop;
}

void UvEventLoop::init()
{
    auto result = uv_loop_init(_eventLoop);
    if (result != 0) {
        // TODO
    }
}

void UvEventLoop::close()
{
    auto result = uv_loop_close(_eventLoop);
    if (result != 0) {
        // TODO
    }
}

void UvEventLoop::run()
{
    uv_run(_eventLoop, UV_RUN_DEFAULT);
}
