#ifndef VULKAN_DEVICE_QUEUE_H
#define VULKAN_DEVICE_QUEUE_H

#include "common/error.h"

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>

class VulkanFunctionTable;

class DeviceQueue;
typedef std::shared_ptr<DeviceQueue> DeviceQueuePtr;

class DeviceQueue
{
public:
    DeviceQueue(std::shared_ptr<VulkanFunctionTable> vft, VkQueue queue);
    virtual ~DeviceQueue();
    
    std::vector<VkQueueFamilyProperties> enumerateQueueFamilyProperties(VkPhysicalDevice physicalDevice);
    
private:
    VkQueue m_queue;

    std::shared_ptr<VulkanFunctionTable> m_functionTable;
};

#endif