#include "vulkan-semaphore.h"

#include <utility>

#include "vulkan.h"
#include "vulkan-function-table.h"

#include <string>

VulkanSemaphore::VulkanSemaphore(
        std::shared_ptr<VulkanFunctionTable> vft,
        VulkanDevicePtr vulkanDevice) :
    _functionTable(std::move(vft)),
    _device(vulkanDevice),
    _initialized(false)
{
}

VulkanSemaphore::~VulkanSemaphore()
{
    // TODO look at vulkan handle?
    if (_initialized) {
       _functionTable->DestroySemaphore(_device->vulkanHandle(), _semaphore, nullptr);
    }
}

ErrorPtr VulkanSemaphore::init()
{
    VkSemaphoreCreateInfo semaphoreInfo = {};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    if (_functionTable->CreateSemaphore(_device->vulkanHandle(), &semaphoreInfo, nullptr, &_semaphore) != VK_SUCCESS) {
        return Error::create("failed to create semaphore!");
    }

    _initialized = true;

    return Error::none();
}


