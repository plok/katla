#ifndef KATLA_GTK_LIST_VIEW_H
#define KATLA_GTK_LIST_VIEW_H

#include "katla/core/error.h"
#include "katla/core/subject.h"

#include "katla/ui-components/list-view.h"

#include "katla/gtk/gtk-widget-interface.h"

#include <functional>

#include <gtk/gtk.h>

namespace katla {

    class GtkListView : public ListView, public GtkWidgetInterface {
    public:
        GtkListView();
        virtual ~GtkListView();

        void init();
        void show();

        void updateContainer(const ContainerState& state) override;

        // TODO replace by builder API?
        void addWidget(std::shared_ptr<Widget> widget);


        // std::unique_ptr<Subscription> onClicked(std::function<void(void)> clickedCallback) override {
        //     return m_onClickedSubject.subscribe(std::make_shared<katla::FuncObserver<void>>(clickedCallback));
        // };

        ::GtkWidget* handle() override {
            return GTK_WIDGET(m_widget);
        }

    private:
        // static void handleClicked(::GtkButton *button, GtkButtonImpl* self);

        ::GtkScrolledWindow *m_widget {};
        ::GtkViewport* m_viewport {};
        ::GtkListBox *m_listWidget {};
        

        // TODO use references instead (state outside widget)
        ContainerState m_state {};

        // katla::Subject<void> m_onClickedSubject;
    };
}

#endif