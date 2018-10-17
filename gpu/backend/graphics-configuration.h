#ifndef GRAPHICS_CONFIGURATION_H
#define GRAPHICS_CONFIGURATION_H

enum class GraphicsBackends {
    OpenGl,
    Vulkan
};

struct GraphicsConfiguration
{
    GraphicsBackends graphicsBackend = GraphicsBackends::OpenGl;
    bool useImGui = false;
    bool useSkia = false;
};

#endif
