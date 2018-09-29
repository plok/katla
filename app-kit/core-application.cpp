#include "core-application.h"

#include "timer-uv.h"

#include <iostream>

CoreApplication::CoreApplication()
{
    _eventLoop = std::make_shared<UvEventLoop>();
    _signalHandler = std::make_shared<UvSignalHandler>(_eventLoop);
}

CoreApplication::~CoreApplication() {

}

ErrorPtr CoreApplication::init()
{
    auto error = _eventLoop->init();
    if (error) {
        return error;
    }

    error = _signalHandler->init();
    if (error) {
        return error;
    }

    error = _signalHandler->start([=](Signal signal) {
        std::cout << "Signal received: " << (int)signal << std::endl << std::flush;
        _eventLoop->close();
    });

    return error;
}

ErrorPtr CoreApplication::run()
{
    return _eventLoop->run();
}

ErrorPtr CoreApplication::close()
{
    _signalHandler->stop();
    return _eventLoop->close();
}

std::shared_ptr<Timer> CoreApplication::createTimer()
{
    auto timer = std::make_shared<UvTimer>(_eventLoop);
    
    timer->init();

    return timer;
}


