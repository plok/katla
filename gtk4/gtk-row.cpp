#include "gtk-row.h"

#include <gtk/gtk.h>

namespace katla {

    GtkRow::GtkRow() {
    }

    GtkRow::~GtkRow() {
        if (m_widget) {
            g_object_unref(m_widget);
            m_widget = nullptr;
        }
    }

    void GtkRow::init() {
        if (m_widget) {
            return;
        }

        m_widget = GTK_BOX(::gtk_box_new(GTK_ORIENTATION_HORIZONTAL, m_state.spacing));
        g_object_ref_sink(m_widget);

        // new defaults
        gtk_box_set_spacing(m_widget, 4);
        gtk_widget_set_margin_start (GTK_WIDGET(m_widget), 4);
        gtk_widget_set_margin_end (GTK_WIDGET(m_widget), 4);
        gtk_widget_set_margin_top (GTK_WIDGET(m_widget), 4);
        gtk_widget_set_margin_bottom (GTK_WIDGET(m_widget), 4);

        // TODO move
//        ::gtk_widget_set_size_request (GTK_WIDGET(m_widget), 0, 40);

        // TODO updateContainer?
        for(auto& containerChild : m_state.children) {
            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();

            // TODO to widget
            int width, height = 0;
            ::gtk_widget_get_size_request(GTK_WIDGET(gtkChild), &width, &height);

            if (containerChild.width.has_value()) {
                width = containerChild.width.value();
                ::gtk_widget_set_hexpand(GTK_WIDGET(gtkChild), false);
            }
            if (containerChild.height.has_value()) {
                height = containerChild.height.value();
            }
            ::gtk_widget_set_size_request(GTK_WIDGET(gtkChild), width, height);

            ::gtk_box_append(m_widget, gtkChild);
            //, containerChild.expand, containerChild.fill, static_cast<uint>(m_state.spacing + containerChild.additionSpacing));
        }
        // g_signal_connect (m_button, "clicked", G_CALLBACK (handleClicked), this);
    }

    void GtkRow::updateContainer(const ContainerState& state)
    {
        if (!m_widget) {
            m_state = state;
            return;
        }

        // TODO: fase1: clear all - fase2: diff
        clear();
        m_state = state;

        for(auto& containerChild : m_state.children) {
            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();

            // TODO to widget
            int width, height = 0;
            ::gtk_widget_get_size_request(GTK_WIDGET(gtkChild), &width, &height);

            if (containerChild.width.has_value()) {
                width = containerChild.width.value();
                ::gtk_widget_set_hexpand(GTK_WIDGET(gtkChild), false);
            }
            if (containerChild.height.has_value()) {
                height = containerChild.height.value();
            }
            ::gtk_widget_set_size_request(GTK_WIDGET(gtkChild), width, height);

            ::gtk_box_append(m_widget, gtkChild);
            // containerChild.expand, containerChild.fill, static_cast<uint>(m_state.spacing + containerChild.additionSpacing));
        }

        // gtk_button_set_label (m_button, state.label.c_str());
    }

    void GtkRow::append(std::shared_ptr<Widget> child) {
        ContainerChild containerChild;
        containerChild.child = child;
        m_state.children.push_back(containerChild);
        
        auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(child.get())->handle();
        ::gtk_box_append(m_widget, gtkChild);
    }

    // TODO clear through state instead?
    void GtkRow::clear() {
        for(auto& containerChild : m_state.children) {
            auto gtkChild = dynamic_cast<katla::GtkWidgetInterface*>(containerChild.child.get())->handle();
            ::gtk_box_remove(m_widget, gtkChild);
        }
        m_state.children.clear();
    }

    void GtkRow::show() {
        gtk_widget_show(GTK_WIDGET(m_widget));
    }

    // void GtkColumn::handleClicked(::GtkButton *button, GtkButtonImpl* self)
    // {
    //     self->m_onClickedSubject.next();
    // }


}