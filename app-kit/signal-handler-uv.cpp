#include "signal-handler-uv.h"

#include "event-loop-uv.h"

#include <iostream>

void uv_signal_handler_callback(uv_signal_t* handle, int signum)
{
    static_cast<UvSignalHandler*>(handle->data)->callback(signum);
}

UvSignalHandler::UvSignalHandler(std::shared_ptr<UvEventLoop> eventLoop)
{
    _eventLoop = eventLoop->uvEventLoop();
    _signalHandler = new uv_signal_t();
    _signalHandler->data = this;
}

UvSignalHandler::~UvSignalHandler() {
    stop();
    delete _signalHandler;
}

ErrorPtr UvSignalHandler::init()
{
    auto result = uv_signal_init(_eventLoop, _signalHandler);
    if (result != 0) {
        return Error::create( uv_err_name(result), uv_strerror(result) );
    }

    return Error::none();
}

ErrorPtr UvSignalHandler::start(std::function<void(Signal)> function)
{
    _function = function;

    auto result = uv_signal_start(_signalHandler, &uv_signal_handler_callback, SIGINT);
    if (result != 0) {
        return Error::create( uv_err_name(result), uv_strerror(result) );
    }

    result = uv_signal_start(_signalHandler, &uv_signal_handler_callback, SIGHUP);
    if (result != 0) {
        return Error::create( uv_err_name(result), uv_strerror(result) );
    }

    return Error::none();
}

ErrorPtr UvSignalHandler::stop()
{
    auto result = uv_signal_stop(_signalHandler);
    if (result != 0) {
        return Error::create( uv_err_name(result), uv_strerror(result) );
    }

    return Error::none();
}

void UvSignalHandler::callback(int signum)
{
    Signal signal = Signal::Unknown;

    switch(signum) {
        case SIGINT: signal = Signal::Interrupt; break;
        case SIGHUP: signal = Signal::Hangup; break;
        default: signal = Signal::Unknown; break;
    }

    // TODO check
    if (_function) {
        _function(signal);
    }
}