#include "app-kit/core-application.h"

#include "ui-components-qt/ui-components-qt.h"

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

    CoreApplication app;
    auto appError = app.init();
    if (appError) {
        std::cout << appError->name << " " << appError->message << std::endl << std::flush;
        return -1;
    }

    UiComponentsQt qtComponents;
    qtComponents.init(argc, argv);

    qtComponents.createWindow();

    qtComponents.run();

    // TODO integrate event-loop or seperate process??
//    appError = app.run();
//    if (appError) {
//        std::cout << appError->name << " " << appError->message << std::endl;
//        return -1;
//    }

    return 0;
}
