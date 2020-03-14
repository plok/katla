#include "core-application-uv.h"

#include "timer-uv.h"

#include "event-loop-uv.h"
#include "signal-handler-uv.h"

#include <iostream>

UvCoreApplication::UvCoreApplication()
{
    _eventLoop = std::make_shared<UvEventLoop>();
    _signalHandler = std::make_shared<UvSignalHandler>(_eventLoop);
}

UvCoreApplication::~UvCoreApplication() = default;

ErrorPtr UvCoreApplication::init()
{
    auto error = _eventLoop->init();
    if (error) {
        return error;
    }

    error = _signalHandler->init();
    if (error) {
        return error;
    }

    error = _signalHandler->start([this](Signal signal) {
        std::cout << "Signal received: " << (int)signal << std::endl << std::flush;
        _eventLoop->close();
    });

    return error;
}

ErrorPtr UvCoreApplication::run()
{
    return _eventLoop->run();
}

ErrorPtr UvCoreApplication::close()
{
    _signalHandler->stop();
    return _eventLoop->close();
}

std::shared_ptr<Timer> UvCoreApplication::createTimer()
{
    auto timer = std::make_shared<UvTimer>(_eventLoop);
    
    timer->init();

    return timer;
}

std::shared_ptr<EventLoop> UvCoreApplication::eventLoop() {
    return _eventLoop;
}
std::shared_ptr<UvEventLoop> UvCoreApplication::uvEventLoop() {
    return _eventLoop;
}

