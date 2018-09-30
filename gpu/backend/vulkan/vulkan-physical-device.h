#ifndef VULKAN_PHYSICAL_DEVICE_H
#define VULKAN_PHYSICAL_DEVICE_H

#include "common/error.h"
#include "gpu/backend/physical-device.h"

#include <vulkan/vulkan_core.h>

#include <memory>
#include <vector>

class VulkanFunctionTable;

class VulkanPhysicalDevice;
typedef std::shared_ptr<VulkanPhysicalDevice> VulkanPhysicalDevicePtr;

class VulkanPhysicalDevice : public PhysicalDevice
{
public:
    VulkanPhysicalDevice(std::shared_ptr<VulkanFunctionTable> functionTable, VkPhysicalDevice physicalDevice);
    virtual ~VulkanPhysicalDevice();
    
    void printInfo();

    ErrorPtr validateForGraphics();

    std::tuple<std::vector<VkExtensionProperties>, ErrorPtr> getExtensions();

    std::vector<VkQueueFamilyProperties> getQueueFamilies();
    void queueFamilyProperties(std::vector<VkQueueFamilyProperties> queueFamilyProperties);

    VkPhysicalDevice vulkanHandle() {
        return m_physicalDevice;
    }
private:
    std::shared_ptr<VulkanFunctionTable> m_functionTable;

    VkPhysicalDevice m_physicalDevice;

};

#endif
