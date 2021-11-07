#ifndef KATLA_GTK_TEXT_IMPL_H
#define KATLA_GTK_TEXT_IMPL_H

#include "katla/core/error.h"
#include "katla/core/subject.h"

#include "katla/ui-components/text.h"

#include "katla/gtk/gtk-widget-interface.h"

#include <functional>

#include <gtk/gtk.h>

namespace katla {

    class GtkTextImpl : public Text, public GtkWidgetInterface {
    public:
        GtkTextImpl();
        virtual ~GtkTextImpl();

        void init();
        void show();

        void updateText(const TextState& state) override;


        // std::unique_ptr<Subscription> onClicked(std::function<void(void)> clickedCallback) override {
        //     return m_onClickedSubject.subscribe(std::make_shared<katla::FuncObserver<void>>(clickedCallback));
        // };

        ::GtkWidget* handle() override {
            return GTK_WIDGET(m_widget);
        }

    private:
        // static void handleClicked(::GtkButton *button, GtkButtonImpl* self);

        ::GtkLabel *m_widget {};

        // TODO use references instead (state outside widget)
        TextState m_state {};

        // katla::Subject<void> m_onClickedSubject;
    };
}

#endif