#include "gtk-button-impl.h"

#include <iostream>
#include <utility>

#include <gtk/gtk.h>

namespace katla {

    GtkButtonImpl::GtkButtonImpl() {
    }

    GtkButtonImpl::~GtkButtonImpl() {
        if (m_widget) {
            g_object_unref(m_widget);
            m_widget = nullptr;
        }
    }

    void GtkButtonImpl::init() {
        if (m_widget) {
            return;
        }

        m_widget = GTK_BUTTON(::gtk_button_new_with_label(m_state.label.c_str()));
        g_object_ref_sink(m_widget);

        g_signal_connect (m_widget, "clicked", G_CALLBACK (handleClicked), this);
    }

    void GtkButtonImpl::updateButton(const ButtonState& state)
    {
        m_state = state;
        if (!m_widget) {
            return;
        }

        gtk_button_set_label (m_widget, state.label.c_str());
    }

    void GtkButtonImpl::show() {
        gtk_widget_show(GTK_WIDGET(m_widget));
    }

    void GtkButtonImpl::handleClicked(::GtkButton *button, GtkButtonImpl* self)
    {
        self->m_onClickedSubject.next();
    }


}