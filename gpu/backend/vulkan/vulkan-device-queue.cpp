#include "vulkan-device-queue.h"

#include "vulkan.h"
#include "vulkan-function-table.h"

DeviceQueue::DeviceQueue(std::shared_ptr<VulkanFunctionTable> vft, VkQueue queue) :
    m_functionTable(vft),
    m_queue(queue)
{
}

DeviceQueue::~DeviceQueue()
{
}
