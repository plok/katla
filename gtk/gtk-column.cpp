#include "gtk-column.h"

#include <gtk/gtk.h>

namespace katla {

    GtkColumn::GtkColumn() {
    }

    GtkColumn::~GtkColumn() {
        if (m_widget) {
            gtk_widget_destroy(GTK_WIDGET(m_widget));
            m_widget = nullptr;
        }
    }

    void GtkColumn::init() {
        if (m_widget) {
            return;
        }

        m_widget = GTK_BOX(::gtk_box_new(GTK_ORIENTATION_VERTICAL, m_state.spacing));

        // TODO updateContainer?
        for(auto& containerChild : m_state.children) {
            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();
            ::gtk_box_pack_start(m_widget, gtkChild, containerChild.expand, containerChild.fill, static_cast<uint>(m_state.spacing + containerChild.additionSpacing));
        }
        // g_signal_connect (m_button, "clicked", G_CALLBACK (handleClicked), this);
    }

    void GtkColumn::updateContainer(const ContainerState& state)
    {
        m_state = state;
        if (!m_widget) {
            return;
        }

        // TODO: fase1: clear all - fase2: diff
        for(auto& containerChild : m_state.children) {
            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();
            ::gtk_container_remove(GTK_CONTAINER(m_widget), gtkChild);
        }

        for(auto& containerChild : m_state.children) {
            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();
            ::gtk_box_pack_start(m_widget, gtkChild, containerChild.expand, containerChild.fill, static_cast<uint>(m_state.spacing + containerChild.additionSpacing));
        }

        // gtk_button_set_label (m_button, state.label.c_str());
    }

    void GtkColumn::show() {
        gtk_widget_show_all(GTK_WIDGET(m_widget));
    }

    // void GtkColumn::handleClicked(::GtkButton *button, GtkButtonImpl* self)
    // {
    //     self->m_onClickedSubject.next();
    // }


}