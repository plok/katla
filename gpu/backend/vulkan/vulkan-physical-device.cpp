#include "vulkan-physical-device.h"

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <sstream>
#include <utility>

VulkanPhysicalDevice::VulkanPhysicalDevice(
        VulkanFunctionTable& vk,
        VkPhysicalDevice physicalDevice) :
    _vk(vk),
    m_physicalDevice(physicalDevice)
{
}

VulkanPhysicalDevice::~VulkanPhysicalDevice() = default;

void VulkanPhysicalDevice::printInfo()
{
    // Return info from device
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures   deviceFeatures;

    _vk.GetPhysicalDeviceProperties( m_physicalDevice, &deviceProperties );
    _vk.GetPhysicalDeviceFeatures( m_physicalDevice, &deviceFeatures );
    
    uint32_t majorVersion = VK_VERSION_MAJOR( deviceProperties.apiVersion );
    uint32_t minorVersion = VK_VERSION_MINOR( deviceProperties.apiVersion );
    uint32_t patchVersion = VK_VERSION_PATCH( deviceProperties.apiVersion );

    std::cout << "Found device: " << deviceProperties.deviceID << " - " << deviceProperties.deviceType << " - " << static_cast<char*>(deviceProperties.deviceName) << std::endl;
    std::cout << "Supporting api: " << majorVersion << "." << minorVersion << "." << patchVersion << std::endl;
}

std::string VulkanPhysicalDevice::name()
{
    VkPhysicalDeviceProperties deviceProperties;

    _vk.GetPhysicalDeviceProperties( m_physicalDevice, &deviceProperties );

    std::stringstream sstream;
    sstream << deviceProperties.deviceID << " - " << deviceProperties.deviceType << " - " << static_cast<char*>(deviceProperties.deviceName);

    return sstream.str();
}

ErrorPtr VulkanPhysicalDevice::validateForGraphics()
{
    // Return info from device
    VkPhysicalDeviceProperties deviceProperties;
    VkPhysicalDeviceFeatures   deviceFeatures;

    _vk.GetPhysicalDeviceProperties( m_physicalDevice, &deviceProperties );
    _vk.GetPhysicalDeviceFeatures( m_physicalDevice, &deviceFeatures );

    uint32_t majorVersion = VK_VERSION_MAJOR( deviceProperties.apiVersion );
    uint32_t minorVersion = VK_VERSION_MINOR( deviceProperties.apiVersion );
    uint32_t patchVersion = VK_VERSION_PATCH( deviceProperties.apiVersion );

    if((majorVersion < 1) &&
       (deviceProperties.limits.maxImageDimension2D < 4096) )
    {
        std::stringstream message; // ugly
        message << "Physical device " << deviceProperties.deviceName << " doesn't support required parameters!";
        return Error::create(message.str());
    }

    auto queueFamilies = getQueueFamilies();
    bool hasGraphicsDevice = false;
    for (auto queue : queueFamilies) {
        if (queue.queueCount && queue.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            hasGraphicsDevice = true;
        }
    }

    if (!hasGraphicsDevice) {
        return Error::create("Could not find device with graphics capability!");
    }

    // TODO validate device if it supports the surface -> UPDATE: now done with glfw in vulkan-device-factory

//    bool hasSurfaceSupport = false;
//    for (auto i = 0; i< queueFamilies.size(); i++) {
//        VkBool32 presentSupport = false;
//        _vk.GetPhysicalDeviceSurfaceSupportKHR(m_physicalDevice, i, surface, &presentSupport);

//        if (presentSupport) {
//            hasSurfaceSupport = true;
//        }
//    }

//    if (!hasSurfaceSupport) {
//        return Error::create("Could not find device that can render to this surface type!");
//    }
    

    return Error::none();
}

std::tuple<std::vector<VkExtensionProperties>, ErrorPtr> VulkanPhysicalDevice::getExtensions()
{
    // Print device extensions
    uint32_t count = 0;
    VkResult result = _vk.EnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &count, nullptr);

    if (result != VK_SUCCESS) {
        std::stringstream message; // ugly
        message << "Error retreiving device extensions: " << result;
        return {
            std::vector<VkExtensionProperties>(),
            Error::create(message.str())
        };
    }
    
    if (!count) {
        std::cout << "No supported device extensions found" << std::endl;
        return {
            std::vector<VkExtensionProperties>(),
            Error::none()
        };
    }

    std::vector<VkExtensionProperties> extensionProperties(count);
    _vk.EnumerateDeviceExtensionProperties(m_physicalDevice, nullptr, &count, extensionProperties.data());

    return {extensionProperties, Error::none()};
}

std::vector<VkQueueFamilyProperties> VulkanPhysicalDevice::getQueueFamilies()
{
    uint32_t count = 0;
    
    _vk.GetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &count, nullptr);
    
    std::vector<VkQueueFamilyProperties> queueFamilyProperties(count);

    if (count) {
        _vk.GetPhysicalDeviceQueueFamilyProperties(m_physicalDevice, &count, queueFamilyProperties.data());
    }

    return queueFamilyProperties;
}

void VulkanPhysicalDevice::queueFamilyProperties(std::vector<VkQueueFamilyProperties> queueFamilyProperties)
{
    std::cout << "Physical device queue families: " << std::endl;
    for (auto queue : queueFamilyProperties) {
        std::stringstream queueTypes;

        if (queue.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueTypes << "GRAPHICS,";
        }
        if (queue.queueFlags & VK_QUEUE_COMPUTE_BIT) {
            queueTypes << "COMPUTE,";
        }
        if (queue.queueFlags & VK_QUEUE_TRANSFER_BIT) {
            queueTypes << "TRANSFER,";
        }
        if (queue.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) {
            queueTypes << "SPARSE_BINDING,";
        }
        if (queue.queueFlags & VK_QUEUE_PROTECTED_BIT) {
            queueTypes << "PROTECTED,";
        }

        std::cout << queue.queueCount << " queues of type: " << queueTypes.str() << std::endl;
    }

}
