#ifndef UV_EVENT_LOOP_H
#define UV_EVENT_LOOP_H

#include "event-loop.h"
#include "common/error.h"

#include <memory>

#include <uv.h>

class UvEventLoop : public EventLoop {
public:
    UvEventLoop();
    virtual ~UvEventLoop();

    ErrorPtr init();
    ErrorPtr run();
    ErrorPtr close();

    uv_loop_t* uvEventLoop() {
        return _eventLoop;
    }
private:
    uv_loop_t* _eventLoop;
};

#endif