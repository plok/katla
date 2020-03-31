#include "ipc-kit.h"

#include "app-kit/core-application-uv.h"

#include "uv.h"

#include <iostream>
#include <utility>

IpcKit::IpcKit(UvCoreApplication& coreApplication) :
    _coreApp(&coreApplication)
{
}

IpcKit::~IpcKit() = default;

ErrorPtr IpcKit::init()
{
    uv_disable_stdio_inheritance();

    return Error::none();
}


