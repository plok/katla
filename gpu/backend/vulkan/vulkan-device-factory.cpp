#include "vulkan-device-factory.h"

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <cstring>
#include <sstream>
#include <utility>

DeviceFactory::DeviceFactory(const VkInstance& instance, std::shared_ptr<VulkanFunctionTable> vft) :
    m_instance(instance),
    m_functionTable(std::move(vft))
{
}

DeviceFactory::~DeviceFactory() = default;

std::tuple<VulkanDevicePtr, ErrorPtr> DeviceFactory::create(VulkanPhysicalDevicePtr physicalDevice)
{
    auto queueFamilies = physicalDevice->getQueueFamilies();

    int queueFamilyIndex = 0;
    bool queueFound = false;

    int presentQueueFamilyIndex = 0;
    bool presentQueueFound = false;

    for (size_t i = 0; i < queueFamilies.size(); i++) {
        auto queue = queueFamilies[i];
        if (queue.queueCount > 0 && queue.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            queueFound = true;
            queueFamilyIndex = i;
        }

        if (queue.queueCount > 0 > 0 &&
            glfwGetPhysicalDevicePresentationSupport(m_instance, physicalDevice->vulkanHandle(), i))
        {
            presentQueueFound = i;
            presentQueueFamilyIndex = i;
        }
    }

    if (!queueFound) {
        return {VulkanDevicePtr(), Error::create("Could not find a graphics queue.")};
    }

    // graphics queue
    VkDeviceQueueCreateInfo createQueueInfo;
    memset(&createQueueInfo, 0, sizeof(createQueueInfo));
    createQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    createQueueInfo.pNext = nullptr;
    createQueueInfo.flags = 0;

    float priorities[] = {0.0f};
    createQueueInfo.queueFamilyIndex = queueFamilyIndex;
    createQueueInfo.queueCount = 1;
    createQueueInfo.pQueuePriorities = static_cast<const float*>(priorities);


    VkPhysicalDeviceFeatures deviceFeatures = {};

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos({createQueueInfo});

    // Create present queue.
    // TODO: relation presentQueue <-> surface <-> device initialization
    if (presentQueueFound && queueFamilyIndex != presentQueueFamilyIndex)
    {
        VkDeviceQueueCreateInfo createPresentQueueInfo;
        memset(&createPresentQueueInfo, 0, sizeof(createPresentQueueInfo));
        createPresentQueueInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        createPresentQueueInfo.pNext = nullptr;
        createPresentQueueInfo.flags = 0;

        float priorities[] = {0.0f};
        createPresentQueueInfo.queueFamilyIndex = presentQueueFamilyIndex;
        createPresentQueueInfo.queueCount = 1;
        createPresentQueueInfo.pQueuePriorities = static_cast<const float*>(priorities);

        queueCreateInfos.push_back(createPresentQueueInfo);
    }

    VkDeviceCreateInfo deviceCreateInfo;
    memset(&deviceCreateInfo, 0, sizeof(deviceCreateInfo));
    deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    deviceCreateInfo.pNext = nullptr;
    deviceCreateInfo.queueCreateInfoCount = 1;
    deviceCreateInfo.pQueueCreateInfos = queueCreateInfos.data();
    deviceCreateInfo.pEnabledFeatures = &deviceFeatures;

    // TODO: create single default value somewhere and allow user to change
    const std::vector<const char*> device_extension_names(
        {VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME, VK_KHR_SWAPCHAIN_EXTENSION_NAME}
    );

    deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(device_extension_names.size());
    deviceCreateInfo.ppEnabledExtensionNames = device_extension_names.data();

    VkDevice vkDevice;
    auto result = m_functionTable->CreateDevice(physicalDevice->vulkanHandle(), &deviceCreateInfo, nullptr, &vkDevice);

    if (result) {
        std::stringstream message;
        message << "Error creating devices: " << (int)result << std::endl;
        return {std::shared_ptr<VulkanDevice>(), Error::create(message.str())};
    }

    auto device = std::make_shared<VulkanDevice>(m_functionTable, vkDevice);

    device->initQueue(queueFamilyIndex, 0);

    return {device, Error::none()};
}
