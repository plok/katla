#include "gtk-text-field-impl.h"

#include <gtk/gtk.h>

namespace katla {

    GtkTextFieldImpl::GtkTextFieldImpl() {
    }

    GtkTextFieldImpl::~GtkTextFieldImpl() {
        if (m_widget) {
            g_object_unref(GTK_WIDGET(m_widget));
            m_widget = nullptr;
        }
    }

    void GtkTextFieldImpl::init() {
        if (m_widget) {
            return;
        }

        m_widget = GTK_ENTRY(::gtk_entry_new(m_state.text.c_str()));
        g_object_ref_sink (m_widget);

        m_buffer = GTK_ENTRYBUFFER(::gtk_entry_buffer_new(m_state.text.c_str(), m_state.text.size()));

        gtk_widget_set_halign (GTK_WIDGET(m_widget), GTK_ALIGN_START);
//        g_signal_connect (m_widget, "clicked", G_CALLBACK (handleClicked), this);
    }

    void GtkTextFieldImpl::setText(const TextFieldState& state)
    {
        m_state = state;
        if (!m_widget) {
            return;
        }

        gtk_entry_set_buffer (m_widget, state.text.c_str());
    }

    void GtkTextFieldImpl::show() {
        gtk_widget_show(GTK_WIDGET(m_widget));
    }

//    void GtkTextImpl::handleClicked(::GtkButton *button, GtkButtonImpl* self)
//    {
//        self->m_onClickedSubject.next();
//    }


}