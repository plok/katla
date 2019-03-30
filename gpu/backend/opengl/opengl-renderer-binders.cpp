//
// Created by simon on 10/10/18.
//

#include "opengl-renderer-binders.h"

// TODO compile env dependend?
#include "render/imgui-glfw-binder.h"

#include <GLFW/glfw3.h>

OpenGlRendererBinders::OpenGlRendererBinders()
{
    _binderMap[ImGuiGlfwOpengl] = ImGuiGlfwBinder::init;
}

void OpenGlRendererBinders::init(OpenGlRenderer implementation, GLFWwindow* window)
{
    auto binder = _binderMap.find(implementation);
    if (binder != _binderMap.end()) {
        _binderMap[implementation](window);
    }
}
