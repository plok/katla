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
    VulkanPhysicalDevice(
        VulkanFunctionTable& vk,
        VkPhysicalDevice physicalDevice);
    virtual ~VulkanPhysicalDevice();
    
    void printInfo();
    std::string name();

    ErrorPtr validateForGraphics();

    std::tuple<std::vector<VkExtensionProperties>, ErrorPtr> getExtensions();

    std::vector<VkQueueFamilyProperties> getQueueFamilies();
    void queueFamilyProperties(std::vector<VkQueueFamilyProperties> queueFamilyProperties);

    VkPhysicalDevice handle() {
        return m_physicalDevice;
    }
private:
    VulkanFunctionTable& _vk;

    VkPhysicalDevice m_physicalDevice;

};

#endif
