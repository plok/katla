#include "vulkan-device.h"

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <sstream>
#include <utility>

VulkanDevice::VulkanDevice(
        std::shared_ptr<VulkanFunctionTable> functionTable,
        VkDevice device) :
    m_functionTable(std::move(functionTable)),
    m_device(device)
{
}

VulkanDevice::~VulkanDevice() = default;

ErrorPtr VulkanDevice::initQueue(uint32_t queueFamilyIndex, uint32_t queueIndex)
{
    VkQueue vkQueue;
    m_functionTable->GetDeviceQueue(m_device, queueFamilyIndex, queueIndex, &vkQueue);

    m_queue = std::make_shared<DeviceQueue>(m_functionTable, vkQueue);

    return Error::none();
}