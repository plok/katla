#ifndef GRAPHICS_CONFIGURATION_H
#define GRAPHICS_CONFIGURATION_H

#include "katla-graphics.h"

enum class KATLA_GRAPHICS_DECLSPEC GraphicsBackends {
    Software,
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
