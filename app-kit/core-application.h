#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

#include "event-loop-uv.h"
#include "signal-handler-uv.h"

#include "timer.h"

#include "common/error.h"

#include <memory>

class CoreApplication {
public:
    CoreApplication();
    virtual ~CoreApplication();

    ErrorPtr init();
    ErrorPtr run();
    ErrorPtr close();

    std::shared_ptr<Timer> createTimer();

    std::shared_ptr<EventLoop> eventLoop() {
        return _eventLoop;
    }
private:
    std::shared_ptr<UvEventLoop> _eventLoop;
    std::shared_ptr<UvSignalHandler> _signalHandler;
};

#endif