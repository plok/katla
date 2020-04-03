//
// Created by simon on 10/10/18.
//

#ifndef KATLA_OPENGL_RENDERERS_H
#define KATLA_OPENGL_RENDERERS_H

#include <map>
#include <functional>
#include <memory>

enum OpenGlRenderer {
  SkiaGlfwOpengl,
  ImGuiGlfwOpengl
};

class GLFWwindow;

class OpenGlRendererBinders {

public:
    OpenGlRendererBinders();

    void init(OpenGlRenderer implementation, GLFWwindow* window);

private:
    std::map<OpenGlRenderer, std::function<void(GLFWwindow*)>> _binderMap;

};

#endif //KATLA_RENDERIMPLEMENTATIONS_H
