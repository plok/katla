#ifndef VULKAN_WINDOW_H
#define VULKAN_WINDOW_H

#include "window/window.h"
#include "common/error.h"

#include <memory>

typedef struct GLFWwindow GLFWwindow;

class VulkanWindow : public Window {
public:
    // Takes ownership of window
    VulkanWindow(GLFWwindow* window);
    virtual ~VulkanWindow();

    void show();
    void render();
    void clear() {}



    void waitForClose();

private:
    GLFWwindow* m_window;
};

#endif