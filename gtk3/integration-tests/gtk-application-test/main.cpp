#include "katla/core/core-application.h"

#include "katla/gtk3/gtk-application.h"

#include "katla/gtk3/gtk-text-impl.h"
#include "katla/gtk3/gtk-button-impl.h"
#include "katla/gtk3/gtk-column.h"
#include "katla/gtk3/gtk-row.h"
#include "katla/gtk3/gtk-list-view.h"

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

//    UvCoreApplication app;
//    auto appError = app.init();
//    if (appError) {
//        std::cout << appError->name << " " << appError->message << std::endl << std::flush;
//        return -1;
//    }

    katla::GtkApplication gtkApplication;
    gtkApplication.init(argc, argv, "gtk-application-test");

    std::shared_ptr<katla::GtkWindowImpl> window;

    gtkApplication.onStartup([&gtkApplication, &window]() {
        katla::printInfo("startup!");

        window = gtkApplication.createWindow();

        auto label = std::make_shared<katla::GtkTextImpl>();
        label->updateText({"hallo buttons:"});
        label->init();

        auto button = std::make_shared<katla::GtkButtonImpl>();
        button->updateButton({"hallo!"});
        button->init();
        button->onClicked([]() {
            katla::printInfo("Hallo!");
        });

        auto button2 = std::make_shared<katla::GtkButtonImpl>();
        button2->updateButton({"hallo 2!"});
        button2->init();
        button2->onClicked([]() {
            katla::printInfo("Hallo again!");
        });

        auto genListEntry = [](std::string text) {
            auto entryLabel = std::make_shared<katla::GtkTextImpl>();
            entryLabel->updateText({text});
            entryLabel->init();

            auto entryButton = std::make_shared<katla::GtkButtonImpl>();
            entryButton->updateButton({katla::format("{} Button", text)});
            entryButton->init();
            entryButton->onClicked([text]() {
                katla::printInfo(katla::format("Clicked on: {}", text));
            });

            auto row = std::make_shared<katla::GtkRow>();
//            row->setMinimumHeight(40);
            row->updateContainer({
                    .children = {
                            katla::ContainerChild{.child = entryLabel, true, true, 16},
                            katla::ContainerChild{.child = entryButton, false, true, 0}
                    }
            });
            row->init();

            return row;
        };

        auto button3 = std::make_shared<katla::GtkButtonImpl>();
        button3->updateButton({"hallo 3!"});
        button3->init();
        button3->onClicked([]() {
            katla::printInfo("Hallo 3!");
        });

        auto button4 = std::make_shared<katla::GtkButtonImpl>();
        button4->updateButton({"hallo 4!"});
        button4->init();
        button4->onClicked([]() {
            katla::printInfo("Hallo again 4!");
        });

        auto listview = std::make_shared<katla::GtkListView>();
        listview->updateContainer({
            .children = {
                    katla::ContainerChild{.child = button3},
                    katla::ContainerChild{.child = button4},
                    katla::ContainerChild{.child = genListEntry("1")},
                    katla::ContainerChild{.child = genListEntry("2")}
            }
        });

        listview->init();

        for(int i=0; i < 100; i++) {
            listview->addWidget(genListEntry(katla::format("Entry #{}", i)));
        }
        listview->show();


        auto column = std::make_shared<katla::GtkColumn>();
        column->init();

        column->updateContainer({
                .children = {
                        katla::ContainerChild{.child = label},
                        katla::ContainerChild{.child = button},
                        katla::ContainerChild{.child = button2},
                        katla::ContainerChild{.child = listview, .expand = true, .fill = true}
                }
        });

        window->updateContainer({
                .children = {katla::ContainerChild{.child = std::static_pointer_cast<katla::Widget>(column)}}
        });
    });

    gtkApplication.onActivate([&window]() {
        window->show();
    });



    return gtkApplication.run();

    // TODO integrate event-loop or seperate process??
//    appError = app.run();
//    if (appError) {
//        std::cout << appError->name << " " << appError->message << std::endl;
//        return -1;
//    }

    return 0;
}
