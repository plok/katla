#ifndef VULKAN_DEVICE_FACTORY_H
#define VULKAN_DEVICE_FACTORY_H

#include "common/error.h"

#include <vulkan/vulkan_core.h>
#include "renderer/gpu/vulkan/vulkan-device.h"
#include "renderer/gpu/vulkan/vulkan-physical-device.h"

#include <memory>
#include <vector>

class VulkanFunctionTable;

class DeviceFactory
{
public:
    DeviceFactory(std::shared_ptr<VulkanFunctionTable> vft);
    virtual ~DeviceFactory();
    
    std::tuple<VulkanDevicePtr, ErrorPtr> create(VulkanPhysicalDevicePtr physicalDevice);

private:

    std::shared_ptr<VulkanFunctionTable> m_functionTable;
};

#endif
