#ifndef EVENT_LOOP_H
#define EVENT_LOOP_H

#include "common/error.h"

#include <memory>

class EventLoop {
public:
    virtual void init() = 0;
    virtual void run() = 0;
    virtual void close() = 0;

private:
    
};

#endif