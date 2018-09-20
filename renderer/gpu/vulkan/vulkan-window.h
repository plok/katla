#ifndef VULKAN_WINDOW_H
#define VULKAN_WINDOW_H

#include "window/window.h"
#include "common/error.h"

#include <vulkan/vulkan_core.h>

#include <memory>

typedef struct GLFWwindow GLFWwindow;

class VulkanWindow;
typedef std::shared_ptr<VulkanWindow> VulkanWindowPtr;

class VulkanWindow : public Window {
public:
    // Takes ownership of window
    VulkanWindow(GLFWwindow* window, VkSurfaceKHR surface);
    virtual ~VulkanWindow();

    void show();
    void render();
    void clear() {}

    VkSurfaceKHR vulkanSurface() {
        return m_surface;
    }

    void waitForClose();

private:
    GLFWwindow* m_window;
    VkSurfaceKHR m_surface;
};

#endif
