#include "vulkan-device-factory.h"

#include "vulkan.h"
#include "vulkan-function-table.h"

#include "string.h"
#include <sstream>

DeviceFactory::DeviceFactory(std::shared_ptr<VulkanFunctionTable> vft) :
    m_functionTable(vft)
{
}

DeviceFactory::~DeviceFactory()
{
}

std::tuple<VulkanDevicePtr, ErrorPtr> DeviceFactory::create(VulkanPhysicalDevicePtr physicalDevice)
{
    auto queueFamilies = physicalDevice->getQueueFamilies();

    int queueFamilyIndex = 0;
    bool queueFound = false;

    for (size_t i = 0; i < queueFamilies.size(); i++) {
        auto queue = queueFamilies[i];
        if (queue.queueCount > 0 && queue.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFound = true;
            queueFamilyIndex = i;
        }
    }

    if (!queueFound) {
        return {VulkanDevicePtr(), Error::create("Could not find a graphics queue.")};
    }

    VkDeviceQueueCreateInfo createQueueInfo;
    memset(&createQueueInfo, 0, sizeof(createQueueInfo));
    createQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    createQueueInfo.pNext = NULL;
    createQueueInfo.flags = 0;

    float priorities[] = {0.0f};
    createQueueInfo.queueFamilyIndex = queueFamilyIndex;
    createQueueInfo.queueCount = 1;
    createQueueInfo.pQueuePriorities = priorities;

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos({createQueueInfo});
    VkDeviceCreateInfo deviceCreateInfo;
    memset(&deviceCreateInfo, 0, sizeof(deviceCreateInfo));
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = NULL;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = NULL;
    deviceCreateInfo.enabledExtensionCount = 0;

    VkDevice vkDevice;
    auto result = m_functionTable->CreateDevice(physicalDevice->vulkanHandle(), &deviceCreateInfo, nullptr, &vkDevice);

    if (!result) {
        std::stringstream message;
        message << "Error while enumerating devices: " << (int)result << std::endl;
        return {std::shared_ptr<VulkanDevice>(), Error::create(message.str())};
    }

    auto device = std::make_shared<VulkanDevice>(m_functionTable, vkDevice);

    device->initQueue(queueFamilyIndex, 0);

    return {device, Error::none()};
}
