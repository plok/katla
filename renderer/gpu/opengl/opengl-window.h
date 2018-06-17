#ifndef OPENGL_WINDOW_H
#define OPENGL_WINDOW_H

#include "window/window.h"
#include "common/error.h"

#include <memory>

typedef struct GLFWwindow GLFWwindow;

class OpenGlWindow : public Window {
public:
    // Takes ownership of window
    OpenGlWindow(GLFWwindow* window);
    virtual ~OpenGlWindow();

    void init();

    void show();
    void render();
    void clear();

    void waitForClose();

private:
    GLFWwindow* m_window;
};

#endif