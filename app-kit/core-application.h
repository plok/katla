#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H

#include "katla-appkit.h"

#include "event-loop.h"
#include "signal-handler.h"

#include "timer.h"

#include "common/error.h"

#include <memory>

class KATLA_APPKIT_DECLSPEC CoreApplication {
public:
    CoreApplication() = default;
    virtual ~CoreApplication() = default;

    virtual ErrorPtr init() = 0;
    virtual ErrorPtr run() = 0;
    virtual ErrorPtr close() = 0;

    virtual std::shared_ptr<Timer> createTimer() = 0;

    virtual std::shared_ptr<EventLoop> eventLoop() = 0;
};

#endif
