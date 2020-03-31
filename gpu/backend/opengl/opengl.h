#ifndef KATLA_OPENGL_H
#define KATLA_OPENGL_H

#include "graphics/graphics-backend.h"
#include "graphics/graphics-configuration.h"

#include "gpu/backend/physical-device.h"
#include "gpu/backend/device.h"

#include "common/error.h"

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
