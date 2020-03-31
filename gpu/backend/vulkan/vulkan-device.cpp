#include "vulkan-device.h"

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <sstream>
#include <utility>

VulkanDevice::VulkanDevice(
        VulkanFunctionTable& vk,
        VkDevice device) :
    _vk(vk),
    _device(device)
{
}

VulkanDevice::~VulkanDevice() = default;

ErrorPtr VulkanDevice::initGraphicsQueue(uint32_t queueFamilyIndex, uint32_t queueIndex)
{
    VkQueue vkQueue;
    _vk.GetDeviceQueue(_device, queueFamilyIndex, queueIndex, &vkQueue);

    m_graphicsQueue = std::make_shared<DeviceQueue>(_vk, vkQueue, queueIndex);

    return Error::none();
}

ErrorPtr VulkanDevice::initPresentQueue(uint32_t queueFamilyIndex, uint32_t queueIndex)
{
    VkQueue vkQueue;
    _vk.GetDeviceQueue(_device, queueFamilyIndex, queueIndex, &vkQueue);

    m_presentQueue = std::make_shared<DeviceQueue>(_vk, vkQueue, queueIndex);

    return Error::none();
}