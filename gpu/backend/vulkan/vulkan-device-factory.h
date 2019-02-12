#ifndef VULKAN_DEVICE_FACTORY_H
#define VULKAN_DEVICE_FACTORY_H

#include "common/error.h"

#include <vulkan/vulkan_core.h>
#include "gpu/backend/vulkan/vulkan-device.h"
#include "gpu/backend/vulkan/vulkan-physical-device.h"

#include <memory>
#include <vector>

class VulkanFunctionTable;

class DeviceFactory
{
public:
    DeviceFactory(
        VulkanFunctionTable& vk,
        const VkInstance& instance);
    virtual ~DeviceFactory();
    
    std::tuple<VulkanDevicePtr, ErrorPtr> create(VulkanPhysicalDevicePtr physicalDevice);

private:
    VkInstance m_instance;
    VulkanFunctionTable& _vk;
};

#endif
