#include "katla/core/core-application.h"

#include "katla/ui/application.h"
//#include "katla/ui-cross/cross-column.h"
//#include "katla/ui-cross/cross-text.h"
// #include "katla/gtk4/gtk-text-impl.h"
// #include "katla/gtk4/gtk-button-impl.h"
// #include "katla/gtk4/gtk-column.h"
// #include "katla/gtk4/gtk-row.h"
// #include "katla/gtk4/gtk-list-view.h"

#include "katla/ui/platform-register.h"
#include "katla/gtk4/gtk-application.h"

#include <string>
#include <vector>
#include <memory>

int main(int argc, char* argv[])
{
    std::vector<std::string> args;
    for (int i=0; i<argc; i++) {
        std::string arg = argv[i];
        args.push_back(arg);
    }

    katla::GtkApplication::registerPlatform();

    auto app = katla::Application::create(argc, argv, "cross-application-test");

    std::shared_ptr<katla::Window> window;

    app->onStartup([&app, &window]() {
        katla::printInfo("startup!");

        window = app->createWindow("testapp", {800, 600});

        auto label = katla::Text::create();
        label->updateText({"hallo buttons:"});

        auto button = katla::Button::create();
        button->updateButton({"hallo!"});
        button->onClicked([]() {
            katla::printInfo("Hallo!");
        });

        auto button2 = katla::Button::create();
        button2->updateButton({"hallo 2!"});
        button2->onClicked([]() {
            katla::printInfo("Hallo again!");
        });

        auto column = katla::Column::create();

        column->updateContainer({
                .children = {
                        katla::ContainerChild{.child = std::move(label)},
                        katla::ContainerChild{.child = std::move(button)},
                        katla::ContainerChild{.child = std::move(button2)},
                }
        });

        window->updateContainer({
                .children = {katla::ContainerChild{.child = std::move(column)}}
        });
    });

    app->onActivate([&window]() {
        window->show();
    });

    return app->run();
}
