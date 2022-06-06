#ifndef KATLA_GTK_LIST_VIEW_H
#define KATLA_GTK_LIST_VIEW_H

#include "katla/core/error.h"
#include "katla/core/subject.h"

#include "katla/ui-components/list-view.h"

#include "katla/gtk3/gtk-widget-interface.h"

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


        void clear();


         std::unique_ptr<Subscription> onRowSelected(std::function<void(int)> rowSelectedCallback) override {
             return m_onRowSelectedSubject.subscribe(std::make_shared<katla::FuncObserver<int>>(rowSelectedCallback));
         };

         // TODO this does not return the GtkListBox!!!
        ::GtkWidget* handle() override {
            return GTK_WIDGET(m_scrolledWindow);
        }

    private:
        static void handleRowSelected(::GtkListBox* listView, ::GtkListBoxRow* row, GtkListView* self);
        static void removeWidget(::GtkWidget *widget, GtkListView* self);

        ::GtkScrolledWindow *m_scrolledWindow {};
        ::GtkViewport* m_viewport {};
        ::GtkListBox *m_listWidget {};
        

        // TODO use references instead (state outside widget)
        ContainerState m_state {};

         katla::Subject<int> m_onRowSelectedSubject;
    };
}

#endif