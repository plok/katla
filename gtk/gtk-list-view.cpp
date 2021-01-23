#include "gtk-list-view.h"

#include <gtk/gtk.h>

namespace katla {

    GtkListView::GtkListView() {
    }

    GtkListView::~GtkListView() {
        if (m_listWidget) {
            gtk_widget_destroy(GTK_WIDGET(m_listWidget));
            m_listWidget = nullptr;
        }
        if (m_viewport) {
            gtk_widget_destroy(GTK_WIDGET(m_viewport));
            m_viewport = nullptr;
        }
        if (m_widget) {
            gtk_widget_destroy(GTK_WIDGET(m_widget));
            m_widget = nullptr;
        }
    }

    void GtkListView::init() {
        if (m_listWidget || m_viewport || m_widget) {
            // TODO assert
            return;
        }

        m_widget = GTK_SCROLLED_WINDOW(::gtk_scrolled_window_new(nullptr, nullptr));
        ::gtk_widget_set_hexpand(GTK_WIDGET(m_widget), true);
        ::gtk_widget_set_vexpand(GTK_WIDGET(m_widget), true);

        m_viewport = GTK_VIEWPORT(::gtk_viewport_new(nullptr, nullptr));
        ::gtk_widget_set_hexpand(GTK_WIDGET(m_viewport), true);
        ::gtk_widget_set_vexpand(GTK_WIDGET(m_viewport), true);
        ::gtk_container_add(GTK_CONTAINER(m_widget), GTK_WIDGET(m_viewport));

        m_listWidget = GTK_LIST_BOX(::gtk_list_box_new());
        ::gtk_container_add(GTK_CONTAINER(m_viewport), GTK_WIDGET(m_listWidget));





////        // TODO move
//        ::gtk_widget_set_size_request (GTK_WIDGET(m_widget), 300, 300);
////        // TODO move
//        ::gtk_widget_set_size_request (GTK_WIDGET(m_viewport), 300, 300);

        // TODO updateContainer?
        for(auto& containerChild : m_state.children) {
            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();
            ::gtk_container_add(GTK_CONTAINER(m_listWidget), gtkChild);  // TODO flex wrapper or properties?
        }


        // g_signal_connect (m_button, "clicked", G_CALLBACK (handleClicked), this);
    }

    void GtkListView::updateContainer(const ContainerState& state)
    {
        m_state = state;
        if (!m_listWidget) {
            return;
        }

        // TODO: fase1: clear all - fase2: diff
        for(auto& containerChild : m_state.children) {
            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();
            ::gtk_container_remove(GTK_CONTAINER(m_listWidget), gtkChild);
        }

        for(auto& containerChild : m_state.children) {
            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();
            ::gtk_container_add(GTK_CONTAINER(m_listWidget), gtkChild);  // TODO flex wrapper or properties?
        }

        // gtk_button_set_label (m_button, state.label.c_str());
    }

    void GtkListView::addWidget(std::shared_ptr<Widget> widget) {
        auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(widget.get())->handle();
        ::gtk_container_add(GTK_CONTAINER(m_listWidget), GTK_WIDGET(gtkChild));

        m_state.children.push_back(ContainerChild{.child=widget});
    }

    void GtkListView::show() {
        gtk_widget_show_all(GTK_WIDGET(m_widget));
    }

    // void GtkColumn::handleClicked(::GtkButton *button, GtkButtonImpl* self)
    // {
    //     self->m_onClickedSubject.next();
    // }


}