#ifndef KATLA_GTK_BUTTON_IMPL_H
#define KATLA_GTK_BUTTON_IMPL_H

#include "katla/core/error.h"
#include "katla/core/subject.h"

#include "katla/ui-components/button.h"

#include "katla/gtk4/gtk-widget-interface.h"

#include <functional>

#include <gtk/gtk.h>

namespace katla {

    class GtkButtonImpl : public Button, public GtkWidgetInterface {
    public:
        GtkButtonImpl();
        virtual ~GtkButtonImpl();

        void init();
        void show();

        void updateButton(const ButtonState& state) override;


        std::unique_ptr<Subscription> onClicked(std::function<void(void)> clickedCallback) override {
            return m_onClickedSubject.subscribe(std::make_shared<katla::FuncObserver<void>>(clickedCallback));
        };

        ::GtkWidget* handle() override {
            return GTK_WIDGET(m_widget);
        }

    private:
        static void handleClicked(::GtkButton *button, GtkButtonImpl* self);

        ::GtkButton *m_widget {};

        // TODO use references instead (state outside widget)
        ButtonState m_state {};

        katla::Subject<void> m_onClickedSubject;
    };
}

#endif