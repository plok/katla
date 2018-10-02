#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include "window/window.h"
#include "common/error.h"

#include "common/subject.h"

#include <memory>

class RenderView;
typedef struct GLFWwindow GLFWwindow;

class OpenGlWindow : public Window {
public:
    // Takes ownership of window
    OpenGlWindow(GLFWwindow* window, std::shared_ptr<RenderView> renderView, std::shared_ptr<WindowProperties> properties);
    virtual ~OpenGlWindow();

    void init();

    void show();
    void render();
    void clear();
    void close();

    void processEvents();

private:
    static void windowSizeCallback(GLFWwindow *window, int width, int height);
    void windowSizeChanged(int width, int height);

    static void windowFocusCallback(GLFWwindow *window);

    static void windowCloseCallback(GLFWwindow *window);
    void windowCloseRequested();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void framebufferSize(int width, int height);

    GLFWwindow* _window;

    bool _closeRequested;

    std::shared_ptr<RenderView> _renderView;
    std::shared_ptr<WindowProperties> _properties;
};

#endif