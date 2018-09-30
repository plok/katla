#ifndef VULKAN_PHYSICAL_DEVICE_FACTORY_H
#define VULKAN_PHYSICAL_DEVICE_FACTORY_H

#include "common/error.h"

#include <vulkan/vulkan_core.h>
#include "gpu/backend/vulkan/vulkan-physical-device.h"

#include <memory>
#include <vector>

class VulkanFunctionTable;

class PhysicalDeviceFactory
{
public:
    PhysicalDeviceFactory(std::shared_ptr<VulkanFunctionTable> vft, const VkInstance& instance);
    virtual ~PhysicalDeviceFactory();
    
    std::tuple<std::vector<VulkanPhysicalDevicePtr>, ErrorPtr> getPhysicalDevices();
    
private:
    const VkInstance m_instance;

    std::shared_ptr<VulkanFunctionTable> m_functionTable;
};

#endif
