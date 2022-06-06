#ifndef KATLA_GTK_WIDGET_INTERFACE_H
#define KATLA_GTK_WIDGET_INTERFACE_H

#include "katla/core/error.h"
#include "katla/core/subject.h"

#include "katla/ui-components/widget.h"

#include <functional>

#include <gtk/gtk.h>


namespace katla {

    class GtkWidgetInterface {
    public:
        // Takes ownership of window
        GtkWidgetInterface() = default;
        virtual ~GtkWidgetInterface() = default;


        virtual ::GtkWidget* handle() = 0;
    };

}

#endif