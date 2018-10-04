#include "vulkan-window-factory.h"
#include "vulkan-window.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

VulkanWindowFactory::VulkanWindowFactory(VkInstance instance) :
    m_instance(instance)
{}

std::tuple<WindowPtr, ErrorPtr> VulkanWindowFactory::create(std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(properties->size.width, properties->size.height, properties->title.c_str(), NULL, NULL);
    if (!window) {
        return {std::shared_ptr<Window>(), Error::create("Error creating window")};
    }

    VkSurfaceKHR surface;
    VkResult err = glfwCreateWindowSurface(m_instance, window, NULL, &surface);
    if (err)
    {
        if (window) {
            glfwDestroyWindow(window);
            window == nullptr;
        }

        // Window surface creation failed
        return {std::shared_ptr<Window>(), Error::create("Error creating window surface")};
    }

    auto vulkanWindow = std::make_shared<VulkanWindow>(window, surface);

    return {vulkanWindow, Error::none()};    
}