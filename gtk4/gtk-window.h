#ifndef KATLA_GTK_WINDOW_H
#define KATLA_GTK_WINDOW_H

#include "katla/window-management/window.h"
#include "katla/core/error.h"

#include "katla/core/subject.h"

#include "katla/ui/container.h"

#include <memory>
#include <functional>

#include <gtk/gtk.h>

// class RenderView;
// typedef struct GLFWwindow GLFWwindow;

namespace katla {

    class GtkWindowImpl : public Window {
    public:
        // Takes ownership of window
        GtkWindowImpl(::GtkWindow *window);

        virtual ~GtkWindowImpl();

        void init();

        void updateContainer(const ContainerState& state) override;

        void show();

        void close();

        void processEvents();

    private:
//    static void windowSizeCallback(GtkWindow* window, int width, int height);
//    void windowSizeChanged(int width, int height);
//
//    static void windowFocusCallback(GtkWindow* window);
//
//    static void windowCloseCallback(GtkWindow* window);
//    void windowCloseRequested();
//
//    static void framebufferSizeCallback(GtkWindow* window, int width, int height);
//    void framebufferSize(int width, int height);

        ::GtkWindow *_window {};

        ContainerState m_containerState {};

        bool _closeRequested;

        // std::shared_ptr<RenderView> _renderView;
        // std::shared_ptr<WindowProperties> _properties;
    };

}

#endif