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
        VulkanFunctionTable& vk,
        VulkanDevice& device);
    virtual ~VulkanCommandPool();
    
    ErrorPtr init();

    VkCommandPool handle() {
        return _commandPool;
    }

private:
    VulkanFunctionTable& _vk;
    VulkanDevice& _device;

    VkCommandPool _commandPool;

    bool _initialized;
};

#endif