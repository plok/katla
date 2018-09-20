#ifndef GRAPHICS_BACKEND_H
#define GRAPHICS_BACKEND_H

#include "common/error.h"
#include "gpu/physical-device.h"
#include "gpu/device.h"

#include <memory>

class WindowFactory;

class GraphicsBackend
{
public:
    GraphicsBackend() = default;
    virtual ~GraphicsBackend() = default;

    virtual ErrorPtr init() = 0;
    virtual void cleanup() = 0;

    virtual ErrorPtr initDevice() = 0;

    // TODO seperate from GraphicsBackend?
    virtual std::unique_ptr<WindowFactory> windowFactory() = 0;
};

#endif
