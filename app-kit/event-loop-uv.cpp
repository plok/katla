#include "event-loop-uv.h"

#include "uv.h"

UvEventLoop::UvEventLoop()
{
    _eventLoop = new uv_loop_t();
}

UvEventLoop::~UvEventLoop() {
    close();
    delete _eventLoop;
}

ErrorPtr UvEventLoop::init()
{
    auto result = uv_loop_init(_eventLoop);
    if (result != 0) {
        return Error::create( uv_err_name(result), uv_strerror(result) );
    }

    return Error::none();
}

ErrorPtr UvEventLoop::close()
{
    auto result = uv_loop_close(_eventLoop);
    if (result != 0) {
        return Error::create( uv_err_name(result), uv_strerror(result) );
    }

    return Error::none();
}

ErrorPtr UvEventLoop::run()
{
    auto result = uv_run(_eventLoop, UV_RUN_DEFAULT);
    if (result != 0) {
        return Error::create( uv_err_name(result), uv_strerror(result) );
    }

    return Error::none();
}
