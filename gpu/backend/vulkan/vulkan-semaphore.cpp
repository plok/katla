#include "vulkan-semaphore.h"

#include <utility>

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <string>

VulkanSemaphore::VulkanSemaphore(
        VulkanFunctionTable& vk,
        VulkanDevice& device) :
    _vk(vk),
    _device(device),
    _initialized(false)
{
}

VulkanSemaphore::~VulkanSemaphore()
{
    // TODO look at vulkan handle?
    if (_initialized) {
       _vk.DestroySemaphore(_device.handle(), _semaphore, nullptr);
    }
}

ErrorPtr VulkanSemaphore::init()
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (_vk.CreateSemaphore(_device.handle(), &semaphoreInfo, nullptr, &_semaphore) != VK_SUCCESS) {
        return Error::create("failed to create semaphore!");
    }

    _initialized = true;

    return Error::none();
}


