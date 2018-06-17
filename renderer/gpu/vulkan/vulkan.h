#ifndef VULKAN_H
#define VULKAN_H

#include "gpu/graphics-backend.h"
#include "common/error.h"

#include <vulkan/vulkan_core.h>

#include <memory>

class WindowFactory;

class VulkanFunctionTable;

class Vulkan : public GraphicsBackend
{
public:
    Vulkan();
    virtual ~Vulkan();

    ErrorPtr init();
    void cleanup();

    std::unique_ptr<WindowFactory> windowFactory();
    
private:
    VkInstance m_instance;

    std::unique_ptr<VulkanFunctionTable> m_functionTable;
};

#endif