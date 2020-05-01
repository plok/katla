#ifndef KATLA_CORE_APPLICATION_H
#define KATLA_CORE_APPLICATION_H

#include "katla/core/core.h"

#include "katla/core/error.h"
#include "katla/core/event-loop.h"
#include "katla/core/signal-handler.h"
#include "katla/core/timer.h"

#include <memory>

namespace katla {

class CoreApplication {
public:
    CoreApplication() = default;
    virtual ~CoreApplication() = default;

    virtual outcome::result<void, Error> init() = 0;
    virtual outcome::result<void, Error> close() = 0;

    virtual outcome::result<void, Error> run() = 0;
    virtual outcome::result<void, Error> stop() = 0;

    virtual outcome::result<std::unique_ptr<Timer>, Error> createTimer() = 0;

    virtual EventLoop& eventLoop() = 0;
};

}

#endif
