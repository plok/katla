#ifndef VULKAN_SWAPCHAIN_RESOURCES_H
#define VULKAN_SWAPCHAIN_RESOURCES_H

#include <vulkan/vulkan.h>

#include <memory>
#include <vector>

struct SwapChainResources
{
    VkSwapchainKHR swapChain;
    VkSurfaceFormatKHR surfaceFormat;
    VkPresentModeKHR presentMode;
    VkExtent2D extent;

    std::vector<VkImageView> swapChainImageViews;
};

#endif
