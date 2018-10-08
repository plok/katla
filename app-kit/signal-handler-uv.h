#ifndef UV_SIGNAL_HANDLER_H
#define UV_SIGNAL_HANDLER_H

#include "signal-handler.h"
#include "common/error.h"

#include <memory>

#include <uv.h>

class UvEventLoop;

class UvSignalHandler : public SignalHandler {
public:
    UvSignalHandler(const std::shared_ptr<UvEventLoop>& eventLoop);
    virtual ~UvSignalHandler();

    ErrorPtr init();
    ErrorPtr start(std::function<void(Signal)>);
    ErrorPtr stop();

    void callback(int signum);
private:
    std::shared_ptr<UvEventLoop> _eventLoop;

    uv_signal_t* _signalHandler;

    std::function<void(Signal)> _function;
};

#endif