#include "vulkan-physical-device-factory.h"

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <sstream>
#include <utility>

PhysicalDeviceFactory::PhysicalDeviceFactory(
        VulkanFunctionTable& vk,
        const VkInstance& instance) :
    _vk(vk),
    m_instance(instance)
{
}

PhysicalDeviceFactory::~PhysicalDeviceFactory() = default;

std::tuple<std::vector<VulkanPhysicalDevicePtr>, ErrorPtr> PhysicalDeviceFactory::getPhysicalDevices()
{
    uint32_t count = 0;

    // Get number of devices
    VkResult result = _vk.EnumeratePhysicalDevices(m_instance, &count, nullptr);

    std::cout << "EnumeratePhysicalDevices" << result << ":" << count << std::endl;

    std::vector<VulkanPhysicalDevicePtr> physicalDevices;

    if (result) {
        std::stringstream message;
        message << "Error while enumerating devices: " << (int)result << std::endl;
        return {physicalDevices, Error::create(message.str())};
    }

    if (count == 0) {
        return {physicalDevices, Error::create("No compatibel gpu device found")};
    }

    std::vector<VkPhysicalDevice> vulkanPhysicalDevices (count);
    result = _vk.EnumeratePhysicalDevices(m_instance, &count, vulkanPhysicalDevices.data());

    for(auto vkPhysicalDevice : vulkanPhysicalDevices) {
        physicalDevices.push_back( std::make_shared<VulkanPhysicalDevice>(_vk, vkPhysicalDevice) );
    }

    return {physicalDevices, Error::none()};
}

