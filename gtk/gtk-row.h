#ifndef KATLA_GTK_ROW_H
#define KATLA_GTK_ROW_H

#include "katla/core/error.h"
#include "katla/core/subject.h"

#include "katla/ui-components/row.h"

#include "katla/gtk/gtk-widget-interface.h"

#include <functional>

#include <gtk/gtk.h>

namespace katla {

    class GtkRow : public Row, public GtkWidgetInterface {
    public:
        GtkRow();
        virtual ~GtkRow();

        void init();
        void show();

        void updateContainer(const ContainerState& state) override;


        // std::unique_ptr<Subscription> onClicked(std::function<void(void)> clickedCallback) override {
        //     return m_onClickedSubject.subscribe(std::make_shared<katla::FuncObserver<void>>(clickedCallback));
        // };

        ::GtkWidget* handle() override {
            return GTK_WIDGET(m_widget);
        }

    private:
        // static void handleClicked(::GtkButton *button, GtkButtonImpl* self);

        ::GtkBox *m_widget {};

        // TODO use references instead (state outside widget)
        ContainerState m_state {};

        // katla::Subject<void> m_onClickedSubject;
    };
}

#endif