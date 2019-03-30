#ifndef GRAPHICS_BACKEND_H
#define GRAPHICS_BACKEND_H

#include "katla-graphics.h"

#include "common/error.h"

#include <memory>

class WindowFactory;

class KATLA_GRAPHICS_DECLSPEC GraphicsBackend
{
public:
    GraphicsBackend() = default;
    virtual ~GraphicsBackend() = default;

    virtual void cleanup() = 0;

    virtual ErrorPtr initDevice() = 0;

    // TODO seperate from GraphicsBackend?
    // TODO use createWindow() instead
    virtual std::unique_ptr<WindowFactory> windowFactory() = 0;
};

#endif
