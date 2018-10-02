#ifndef VULKAN_WINDOW_H
#define VULKAN_WINDOW_H

#include "gpu/window/window.h"
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

    void init() {};
    void show();
    void render();
    void clear() {}
    void close() {}

    void processEvents() {}

    VkSurfaceKHR vulkanSurface() {
        return m_surface;
    }

    void waitForClose();

    void attachCloseSubject(std::shared_ptr<Subject<bool>> closeSubject) {
        m_closeSubject = closeSubject;
    }

private:
    GLFWwindow* m_window;
    VkSurfaceKHR m_surface;

    std::shared_ptr<Subject<bool>> m_closeSubject;
};

#endif
