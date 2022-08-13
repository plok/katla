#ifndef KATLA_GTK_TEXT_FIELD_IMPL_H
#define KATLA_GTK_TEXT_FIELD_IMPL_H

#include "katla/core/error.h"
#include "katla/core/subject.h"

#include "katla/ui/text-field.h"

#include "katla/gtk4/gtk-widget-interface.h"

#include <functional>

#include <gtk/gtk.h>

namespace katla {

    class GtkTextFieldImpl : public TextField, public GtkWidgetInterface {
    public:
        GtkTextFieldImpl();
        virtual ~GtkTextFieldImpl();

        void init();
        void show() override;

        void setText(const TextFieldState& state) override;


        // std::unique_ptr<Subscription> onClicked(std::function<void(void)> clickedCallback) override {
        //     return m_onClickedSubject.subscribe(std::make_shared<katla::FuncObserver<void>>(clickedCallback));
        // };

        ::GtkWidget* handle() override {
            return GTK_WIDGET(m_widget);
        }

    private:
        // static void handleClicked(::GtkButton *button, GtkButtonImpl* self);

        ::GtkEntry *m_widget {};
        ::GtkEntryBuffer *m_buffer{};

        // TODO use references instead (state outside widget)
        TextFieldState m_state {};

        // katla::Subject<void> m_onClickedSubject;
    };
}

#endif