#ifndef VULKAN_DEVICE_H
#define VULKAN_DEVICE_H

#include "common/error.h"

#include <vulkan/vulkan_core.h>

#include "renderer/gpu/device.h"
#include "renderer/gpu/vulkan/vulkan-device-queue.h"

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
    
    ErrorPtr initQueue(uint32_t queueFamilyIndex, uint32_t queueIndex);

private:
    std::shared_ptr<VulkanFunctionTable> m_functionTable;

    DeviceQueuePtr m_queue;
    VkDevice m_device;
};

#endif
