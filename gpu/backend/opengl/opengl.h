#ifndef OPENGL_H
#define OPENGL_H

#include "gpu/backend/graphics-backend.h"
#include "gpu/backend/physical-device.h"
#include "gpu/backend/device.h"

#include "gpu/backend/graphics-configuration.h"

#include "common/error.h"

#include <vulkan/vulkan_core.h>

#include <memory>

class WindowFactory;

class OpenGl : public GraphicsBackend
{
public:
    OpenGl();
    virtual ~OpenGl();

    ErrorPtr init(const GraphicsConfiguration& openGlConfiguration);
    void cleanup();

    ErrorPtr initDevice() {
        return Error::none();
    }

    std::unique_ptr<WindowFactory> windowFactory();
    
private:
    GraphicsConfiguration _openGlConfiguration;
};

#endif
