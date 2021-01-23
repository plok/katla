#include "app-kit/core-application.h"

#include "ui-components-gtk/ui-components-gtk.h"

#include "app-kit/core-application-uv.h"

#include "string.h"
#include <iostream>
#include <vector>

int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    for (int i=0; i<argc; i++) {
        std::string arg = argv[i];
        args.push_back(arg);
    }

    UvCoreApplication app;
    auto appError = app.init();
    if (appError) {
        std::cout << appError->name << " " << appError->message << std::endl << std::flush;
        return -1;
    }

    UiComponentsGtk gtkComponents;
    gtkComponents.init(argc, argv);

    return gtkComponents.run();

    // TODO integrate event-loop or seperate process??
//    appError = app.run();
//    if (appError) {
//        std::cout << appError->name << " " << appError->message << std::endl;
//        return -1;
//    }

    return 0;
}
