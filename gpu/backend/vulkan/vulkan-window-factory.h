#ifndef VULKAN_WINDOW_FACTORY_H
#define VULKAN_WINDOW_FACTORY_H

#include "gpu/backend/vulkan/vulkan-window.h"

#include "window/window.h"
#include "window/window-factory.h"
#include "common/error.h"

#include "gpu/render/render-view.h"

#include <memory>
#include <string>

#include <vulkan/vulkan_core.h>

class WindowScene;

class VulkanWindowFactory : public WindowFactory {
public:
    // Takes ownership
    VulkanWindowFactory(VkInstance instance);

    std::tuple<WindowPtr, ErrorPtr> create(std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties);

private:
    VkInstance m_instance;
};

#endif