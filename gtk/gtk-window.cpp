#include "gtk-window.h"

#include <iostream>
#include <utility>

#include "gtk-button-impl.h"

#include "katla/gtk/gtk-widget-interface.h"

#include <gtk/gtk.h>

namespace katla {

    GtkWindowImpl::GtkWindowImpl(
            ::GtkWindow *window) :
            _window(window),
            _closeRequested(false) {
    }

    GtkWindowImpl::~GtkWindowImpl() {
        if (_window) {
            gtk_widget_destroy(GTK_WIDGET(_window));

            // glfwDestroyWindow(_window);
            _window = nullptr;
        }
    }

    void GtkWindowImpl::init() {
        if (!_window) {
            return;
        }

        // glfwSetWindowUserPointer(_window, this);

        // glfwSetWindowSizeCallback(_window, &windowSizeCallback);
        // glfwSetWindowCloseCallback(_window, &windowCloseCallback);
        // glfwSetFramebufferSizeCallback(_window, &framebufferSizeCallback);
    }

    void GtkWindowImpl::updateContainer(const ContainerState& state)
    {
        m_containerState = state;
        if (!_window) {
            return;
        }

        if (!state.children.empty()) {
            gtk_container_add(GTK_CONTAINER (_window), GTK_WIDGET(dynamic_cast<katla::GtkWidgetInterface*>(state.children.front().child.get())->handle()));
        }

        gtk_widget_show_all(GTK_WIDGET(_window));

    }

    void GtkWindowImpl::show() {
        int width = 0, height = 0;
        // glfwGetFramebufferSize(_window, &width, &height);

        // glViewport(0, 0, width, height);

        gtk_widget_show_all(GTK_WIDGET(_window));
    }


    void GtkWindowImpl::close() {
        if (!_window) {
            return;
        }

        _closeRequested = true;
        // TODO do not call from callback
        // glfwDestroyWindow (_window);
    }

    void GtkWindowImpl::processEvents() {
        // glfwPollEvents();
    }

//void GtkWindow::windowSizeCallback(GLFWwindow *window, int width, int height)
//{
//     auto userPtr = glfwGetWindowUserPointer(window);
//     if (!userPtr) {
//         return;
//     }
//
//     auto windowObj = static_cast<GlfwWindow*>(userPtr);
//
//     windowObj->windowSizeChanged(width, height);
//}

//void GtkWindow::windowSizeChanged(int width, int height)
//{
    // std::cout << "size" << std::endl;

    // TODO schedule, not in callback
// if (_renderView) {
//     _renderView->resize(width, height);
// }

    // TODO needed, not in callback?
    //render();

    // if (_events->sizeChanged) {

    //     Size size {width, height};
    //     _events->sizeChanged->publish(size);
    // }
//}

// void GtkWindow::windowFocusCallback(GLFWwindow * /*window*/)
// {
//     std::cout << "focus" << std::endl;
// }

// void GtkWindow::windowCloseCallback(GLFWwindow *window)
// {
//     auto userPtr = glfwGetWindowUserPointer(window);
//     if (!userPtr) {
//         return;
//     }

//     auto windowObj = static_cast<GlfwWindow*>(userPtr);

//     windowObj->windowCloseRequested();
// }

// void GtkWindow::windowCloseRequested()
// {
//     _closeRequested = false;

//     if (_properties->closeRequested) {
//         _properties->closeRequested->publish(true);
//     }
// }

// void GtkWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height)
// {
//     // std::cout << "fb size" << std::endl;

//     auto userPtr = glfwGetWindowUserPointer(window);
//     if (!userPtr) {
//         return;
//     }
//     auto windowObj = static_cast<GlfwWindow*>(userPtr);

//     windowObj->framebufferSize(width, height);
// }

//void GtkWindow::framebufferSize(int width, int height)
//{
    // glViewport(0, 0, width, height);
//    render();
//}

}