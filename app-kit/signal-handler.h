#ifndef SIGNAL_HANDLER_H
#define SIGNAL_HANDLER_H

#include "katla-appkit.h"

#include "common/error.h"

#include <memory>
#include <functional>

enum class Signal { Unknown, Interrupt, Hangup };

class KATLA_APPKIT_DECLSPEC SignalHandler {
public:
    virtual ErrorPtr init() = 0;
    virtual ErrorPtr start(std::function<void(Signal)>) = 0;
    virtual ErrorPtr stop() = 0;
};

#endif