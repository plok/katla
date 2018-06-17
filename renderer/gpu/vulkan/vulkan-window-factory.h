#ifndef VULKAN_WINDOW_FACTORY_H
#define VULKAN_WINDOW_FACTORY_H

#include "gpu/vulkan/vulkan-window.h"

#include "window/window.h"
#include "window/window-factory.h"
#include "common/error.h"

#include <memory>
#include <string>

#include <vulkan/vulkan_core.h>

class VulkanWindowFactory : public WindowFactory {
public:
    // Takes ownership
    VulkanWindowFactory(VkInstance instance);

    std::tuple<WindowPtr, ErrorPtr> create(int x, int y, std::string title);

private:
    VkInstance m_instance;
};

#endif