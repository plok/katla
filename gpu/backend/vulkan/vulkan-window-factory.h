#ifndef VULKAN_WINDOW_FACTORY_H
#define VULKAN_WINDOW_FACTORY_H

#include "gpu/backend/vulkan/vulkan-window.h"
#include "gpu/backend/vulkan/vulkan-physical-device.h"

#include "window-management/window.h"
#include "window-management/window-factory.h"
#include "common/error.h"

#include "graphics/render-view.h"

#include <memory>
#include <string>

#include <vulkan/vulkan.h>

class WindowScene;

class VulkanWindowFactory : public WindowFactory {
public:
    // Takes ownership
    VulkanWindowFactory(
        VulkanFunctionTable& vk,
        VkInstance& instance,
        VulkanDevice& device,
        VulkanPhysicalDevice& physicalDevice
        );

    std::tuple<WindowPtr, ErrorPtr> create(std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties);

private:
    SwapChainResources createSwapChain(VkSurfaceKHR surface, std::shared_ptr<WindowProperties> properties);

    VkSurfaceFormatKHR chooseSurfaceFormat(VkSurfaceKHR surface);
    VkPresentModeKHR chooseSwapPresentMode(VkSurfaceKHR surface);
    VkExtent2D chooseSwapExtent(VkSurfaceKHR surface, const VkSurfaceCapabilitiesKHR& capabilities, std::shared_ptr<WindowProperties> properties);

    VulkanFunctionTable& _vk;
    VkInstance& _instance;

    // TODO: move to window?, as there doesn't have to be a single device for all windows
    VulkanPhysicalDevice& _physicalDevice;
    VulkanDevice& _device;
};

#endif