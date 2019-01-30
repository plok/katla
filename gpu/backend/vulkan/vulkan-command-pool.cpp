#include "vulkan-command-pool.h"

#include <utility>

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <string>

VulkanCommandPool::VulkanCommandPool(
        std::shared_ptr<VulkanFunctionTable> vft,
        VulkanDevicePtr vulkanDevice) :
    _functionTable(std::move(vft)),
    _vulkanDevice(vulkanDevice),
    _initialized(false)
{
}

VulkanCommandPool::~VulkanCommandPool()
{
    // TODO look at vulkan handle?
    if (_initialized) {
       _functionTable->DestroyCommandPool(_vulkanDevice->vulkanHandle(), _commandPool, nullptr);
    }
}

ErrorPtr VulkanCommandPool::init()
{   
    _commandPool = {};

    std::cout << "Graphics queue: " << _vulkanDevice->graphicsQueue()->index() << std::endl;

    VkCommandPoolCreateInfo poolInfo = {};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = _vulkanDevice->graphicsQueue()->index();
    poolInfo.flags = 0; // Optional

    if (_functionTable->CreateCommandPool(_vulkanDevice->vulkanHandle(), &poolInfo, nullptr, &_commandPool) != VK_SUCCESS) {
        return Error::create("failed to create command pool!");
    }

    std::cout << "Command buffers created!" << std::endl;

    _initialized = true;

    return Error::none();
}

