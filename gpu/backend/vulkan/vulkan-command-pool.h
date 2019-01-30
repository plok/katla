#ifndef VULKAN_COMMAND_POOL_H
#define VULKAN_COMMAND_POOL_H

#include "common/error.h"

#include "gpu/backend/vulkan/vulkan-device.h"
#include "gpu/backend/vulkan/vulkan-render-pass.h"
#include "gpu/backend/vulkan/vulkan-framebuffer.h"

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>
#include <cstdint>

class VulkanFunctionTable;

class VulkanCommandPool;
typedef std::shared_ptr<VulkanCommandPool> VulkanCommandPoolPtr;

class VulkanCommandPool
{
public:
    VulkanCommandPool(
        std::shared_ptr<VulkanFunctionTable> vft,
        VulkanDevicePtr vulkanDevice);
    virtual ~VulkanCommandPool();
    
    ErrorPtr init();

    VkCommandPool vulkanHandle() {
        return _commandPool;
    }

private:
    std::shared_ptr<VulkanFunctionTable> _functionTable;
    VulkanDevicePtr _vulkanDevice;

    VkCommandPool _commandPool;

    bool _initialized;
};

#endif