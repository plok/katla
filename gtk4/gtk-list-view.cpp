#include "gtk-list-view.h"

#include <gtk/gtk.h>

namespace katla {

    GtkListView::GtkListView() {
    }

    GtkListView::~GtkListView() {
        if (m_listWidget) {
            g_object_unref(m_listWidget);
            m_listWidget = nullptr;
        }
        if (m_viewport) {
            g_object_unref(m_viewport);
            m_viewport = nullptr;
        }
        if (m_scrolledWindow) {
            g_object_unref(m_scrolledWindow);
            m_scrolledWindow = nullptr;
        }
    }

    void GtkListView::init() {
        if (m_listWidget || m_viewport || m_scrolledWindow) {
            // TODO assert
            return;
        }

        m_scrolledWindow = GTK_SCROLLED_WINDOW(::gtk_scrolled_window_new());
        g_object_ref_sink(m_scrolledWindow);
        ::gtk_widget_set_hexpand(GTK_WIDGET(m_scrolledWindow), true);
        ::gtk_widget_set_vexpand(GTK_WIDGET(m_scrolledWindow), true);

        m_viewport = GTK_VIEWPORT(::gtk_viewport_new(nullptr, nullptr));
        g_object_ref_sink(m_viewport);
        ::gtk_widget_set_hexpand(GTK_WIDGET(m_viewport), true);
        ::gtk_widget_set_vexpand(GTK_WIDGET(m_viewport), true);
        ::gtk_scrolled_window_set_child(m_scrolledWindow, GTK_WIDGET(m_viewport));

        m_listWidget = GTK_LIST_BOX(::gtk_list_box_new());
        g_object_ref_sink(m_listWidget);
        ::gtk_viewport_set_child(m_viewport, GTK_WIDGET(m_listWidget));

        g_signal_connect (m_listWidget, "row-selected", G_CALLBACK (handleRowSelected), this);

        // TODO updateContainer?
        for(auto& containerChild : m_state.children) {
            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();
            ::gtk_list_box_append(m_listWidget, gtkChild);  // TODO flex wrapper or properties?
        }

    }

    void GtkListView::updateContainer(const ContainerState& state)
    {
        if (!m_listWidget) {
            m_state = state;
            return;
        }

        // TODO: fase1: clear all - fase2: diff
        for(auto& containerChild : m_state.children) {
            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();
            ::gtk_list_box_remove(m_listWidget, gtkChild);
        }

        m_state = state;

        for(auto& containerChild : m_state.children) {
            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();
            ::gtk_list_box_append(m_listWidget, gtkChild);  // TODO flex wrapper or properties?
        }

        // gtk_button_set_label (m_button, state.label.c_str());
    }

    void GtkListView::append(std::shared_ptr<Widget> child) {
        auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(child.get())->handle();

        ContainerChild containerChild;
        containerChild.child = child;
        m_state.children.push_back(containerChild);

        ::gtk_list_box_append(m_listWidget, gtkChild);
    }

    // TODO clear through state instead?
    void GtkListView::clear() {
//        for(auto& containerChild : m_state.children) {
//            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();
//            ::gtk_container_remove(GTK_CONTAINER(m_listWidget), gtkChild);
//        }

        // ::gtk_container_foreach(GTK_CONTAINER(m_listWidget), GtkCallback(removeWidget), this);
        // m_state.children.clear();
    }

    void GtkListView::show() {
        gtk_widget_show(GTK_WIDGET(m_scrolledWindow));
    }

     void GtkListView::handleRowSelected(::GtkListBox* listView, ::GtkListBoxRow* row, GtkListView* self)
     {
         katla::printInfo("Row selected");

        auto index = ::gtk_list_box_row_get_index(row);
        self->m_onRowSelectedSubject.next(index);
     }

    void GtkListView::removeWidget(::GtkWidget *widget, GtkListView* self) {
        ::gtk_list_box_remove(self->m_listWidget, widget);
    }


}