#ifndef CORE_APPLICATION_UV_H
#define CORE_APPLICATION_UV_H

#include "katla-appkit.h"

#include "event-loop.h"
#include "signal-handler.h"

#include "timer.h"

#include "common/error.h"

#include "core-application.h"

#include <memory>

class UvEventLoop;
class UvSignalHandler;

class KATLA_APPKIT_DECLSPEC UvCoreApplication : public CoreApplication {
public:
    UvCoreApplication();
    virtual ~UvCoreApplication();

    ErrorPtr init() override;
    ErrorPtr run() override;
    ErrorPtr close() override;

    std::shared_ptr<Timer> createTimer() override;

    std::shared_ptr<EventLoop> eventLoop() override;
    std::shared_ptr<UvEventLoop> uvEventLoop();
private:
    std::shared_ptr<UvEventLoop> _eventLoop;
    std::shared_ptr<UvSignalHandler> _signalHandler;
};

#endif
