#ifndef OPENGL_H
#define OPENGL_H

#include "gpu/graphics-backend.h"
#include "common/error.h"

#include <vulkan/vulkan_core.h>

#include <memory>

class WindowFactory;

class OpenGl : public GraphicsBackend
{
public:
    OpenGl();
    virtual ~OpenGl();

    ErrorPtr init();
    void cleanup();

    std::unique_ptr<WindowFactory> windowFactory();
    
private:

};

#endif