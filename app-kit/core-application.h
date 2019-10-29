#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

#include "katla-appkit.h"

#include "event-loop.h"
#include "signal-handler.h"

#include "timer.h"

#include "common/error.h"

#include <memory>

class UvEventLoop;
class UvSignalHandler;

class KATLA_APPKIT_DECLSPEC CoreApplication {
public:
    CoreApplication();
    virtual ~CoreApplication();

    ErrorPtr init();
    ErrorPtr run();
    ErrorPtr close();

    std::shared_ptr<Timer> createTimer();

    std::shared_ptr<EventLoop> eventLoop();
private:
    std::shared_ptr<UvEventLoop> _eventLoop;
    std::shared_ptr<UvSignalHandler> _signalHandler;
};

#endif
