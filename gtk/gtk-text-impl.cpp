#include "gtk-text-impl.h"

#include <gtk/gtk.h>

namespace katla {

    GtkTextImpl::GtkTextImpl() {
    }

    GtkTextImpl::~GtkTextImpl() {
        if (m_widget) {
            gtk_widget_destroy(GTK_WIDGET(m_widget));
            m_widget = nullptr;
        }
    }

    void GtkTextImpl::init() {
        if (m_widget) {
            return;
        }

        m_widget = GTK_LABEL(::gtk_label_new(m_state.text.c_str()));
        g_object_ref(m_widget);

        gtk_widget_set_halign (GTK_WIDGET(m_widget), GTK_ALIGN_START);
//        g_signal_connect (m_widget, "clicked", G_CALLBACK (handleClicked), this);
    }

    void GtkTextImpl::updateText(const TextState& state)
    {
        m_state = state;
        if (!m_widget) {
            return;
        }

        gtk_label_set_text (m_widget, state.text.c_str());
    }

    void GtkTextImpl::show() {
        gtk_widget_show_all(GTK_WIDGET(m_widget));
    }

//    void GtkTextImpl::handleClicked(::GtkButton *button, GtkButtonImpl* self)
//    {
//        self->m_onClickedSubject.next();
//    }


}