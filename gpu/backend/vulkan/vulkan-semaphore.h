#ifndef VULKAN_SEMAPHORE_H
#define VULKAN_SEMAPHORE_H

#include "common/error.h"

#include "gpu/backend/vulkan/vulkan-device.h"
#include "gpu/backend/vulkan/vulkan-render-pass.h"
#include "gpu/backend/vulkan/vulkan-framebuffer.h"
#include "gpu/backend/vulkan/vulkan-command-pool.h"
#include "gpu/backend/vulkan/vulkan-graphics-pipeline.h"

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>
#include <cstdint>

class VulkanFunctionTable;

class VulkanSemaphore;
typedef std::shared_ptr<VulkanSemaphore> VulkanSemaphorePtr;

class VulkanSemaphore
{
public:
    VulkanSemaphore(
        std::shared_ptr<VulkanFunctionTable> vft,
        VulkanDevicePtr vulkanDevice);
    virtual ~VulkanSemaphore();
    
    ErrorPtr init();

    VkSemaphore vulkanHandle() {
        return _semaphore;
    }

private:

    std::shared_ptr<VulkanFunctionTable> _functionTable;
    VulkanDevicePtr _device;

    VkSemaphore _semaphore;

    bool _initialized;
};

#endif