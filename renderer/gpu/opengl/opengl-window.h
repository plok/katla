#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include "window/window.h"
#include "common/error.h"

#include "common/subject.h"

#include <memory>

typedef struct GLFWwindow GLFWwindow;

class OpenGlWindow : public Window {
public:
    // Takes ownership of window
    OpenGlWindow(GLFWwindow* window, std::shared_ptr<WindowEvents> events);
    virtual ~OpenGlWindow();

    void init();

    void show();
    void render();
    void clear();
    void close();

private:
    static void windowSizeCallback(GLFWwindow *window, int width, int height);
    void windowSizeChanged(int width, int height);

    static void windowFocusCallback(GLFWwindow *window);

    static void windowCloseCallback(GLFWwindow *window);
    void windowCloseRequested();

    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    void framebufferSize(int width, int height);

    GLFWwindow* m_window;

    bool m_closeRequested;

    std::shared_ptr<WindowEvents> m_events;
};

#endif