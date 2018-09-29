#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include "common/error.h"

#include <memory>
#include <functional>

enum class Signal { Unknown, Interrupt, Hangup };

class SignalHandler {
public:
    virtual ErrorPtr init() = 0;
    virtual ErrorPtr start(std::function<void(Signal)>) = 0;
    virtual ErrorPtr stop() = 0;
};

#endif