#ifndef KATLA_CORE_EVENT_LOOP_H
#define KATLA_CORE_EVENT_LOOP_H

#include "katla/core/error.h"

class EventLoop {
public:
    virtual ~EventLoop() = default;

    virtual outcome::result<void, Error> init() = 0;
    virtual outcome::result<void, Error> run() = 0;
    virtual outcome::result<void, Error> close() = 0;
private:
    
};

#endif
