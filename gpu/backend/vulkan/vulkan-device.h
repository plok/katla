#ifndef VULKAN_DEVICE_H
#define VULKAN_DEVICE_H

#include "common/error.h"

#include <vulkan/vulkan_core.h>

#include "gpu/backend/device.h"
#include "gpu/backend/vulkan/vulkan-device-queue.h"

#include <memory>
#include <vector>

class VulkanFunctionTable;

class VulkanDevice;
typedef std::shared_ptr<VulkanDevice> VulkanDevicePtr;

class VulkanDevice : public Device
{
public:
    VulkanDevice(std::shared_ptr<VulkanFunctionTable> functionTable, VkDevice device);
    virtual ~VulkanDevice();
    
    ErrorPtr initGraphicsQueue(uint32_t queueFamilyIndex, uint32_t queueIndex);
    ErrorPtr initPresentQueue(uint32_t queueFamilyIndex, uint32_t queueIndex);

    VkDevice vulkanHandle() {
        return m_device;
    }

    DeviceQueuePtr graphicsQueue() {
        return m_graphicsQueue;
    }

    DeviceQueuePtr presentQueue() {
        return m_presentQueue;
    }
private:
    std::shared_ptr<VulkanFunctionTable> m_functionTable;
    VkDevice m_device;

    DeviceQueuePtr m_graphicsQueue;
    DeviceQueuePtr m_presentQueue;
};

#endif
