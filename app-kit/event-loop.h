#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "common/error.h"

#include <memory>

class EventLoop {
public:
    virtual ErrorPtr init() = 0;
    virtual ErrorPtr run() = 0;
    virtual ErrorPtr close() = 0;

private:
    
};

#endif