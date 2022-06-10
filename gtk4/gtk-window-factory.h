#ifndef GTK_WINDOW_FACTORY_H
#define GTK_WINDOW_FACTORY_H

#include "katla/core/core.h"
#include "katla/core/error.h"
#include "katla/core/size.h"

#include "window-management/window.h"
#include "window-management/window-factory.h"

#include <memory>
#include <string>

#include "gtk-window.h"

namespace katla {

class GtkWindowFactory {
public:
    GtkWindowFactory(::GtkApplication* app);

    outcome::result<std::unique_ptr<GtkWindowImpl>, Error> create(std::string title, Size_32s size);
private:
//    GraphicsConfiguration _configuration;

    ::GtkApplication* _app {};

};

}

#endif