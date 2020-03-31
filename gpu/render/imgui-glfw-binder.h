#ifndef KATLA_IMGUI_GLFW_BINDER_H
#define KATLA_IMGUI_GLFW_BINDER_H

#include <GL/glew.h>    // Initialize with glewInit()
#include <GLFW/glfw3.h>

class ImGuiGlfwBinder
{
public:
    ImGuiGlfwBinder();

    static void init(GLFWwindow* window);
};

#endif