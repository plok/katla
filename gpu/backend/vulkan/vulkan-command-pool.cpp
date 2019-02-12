#include "vulkan-command-pool.h"

#include <utility>

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <string>

VulkanCommandPool::VulkanCommandPool(
        VulkanFunctionTable& vk,
        VulkanDevice& device) :
    _vk(vk),
    _device(device),
    _initialized(false)
{
}

VulkanCommandPool::~VulkanCommandPool()
{
    // TODO look at vulkan handle?
    if (_initialized) {
       _vk.DestroyCommandPool(_device.handle(), _commandPool, nullptr);
    }
}

ErrorPtr VulkanCommandPool::init()
{   
    _commandPool = {};

    std::cout << "Graphics queue: " << _device.graphicsQueue()->index() << std::endl;

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = _device.graphicsQueue()->index();
    poolInfo.flags = 0; // Optional

    if (_vk.CreateCommandPool(_device.handle(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
        return Error::create("failed to create command pool!");
    }

    std::cout << "Command buffers created!" << std::endl;

    _initialized = true;

    return Error::none();
}

