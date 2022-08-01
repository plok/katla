#include "katla/ui/c-api/ui.h"
#include "katla/gtk4/c-api/gtk-application.h"

#include "stdio.h"
#include <assert.h>

void* window = 0;

void on_startup(void* app)
{
        void* text = 0;
        void* column = 0;

        printf("startup!");

        struct cSize_32s size;
        size.width = 800;
        size.height = 600;
        window = application_create_window(app, "testapp", size);

        text = text_create("hallo buttons:"); // TODO delete

        // auto button = katla::Button::create();
        // button->updateButton({"hallo!"});
        // button->onClicked([]() {
        //     katla::printInfo("Hallo!");
        // });

        // auto button2 = katla::Button::create();
        // button2->updateButton({"hallo 2!"});
        // button2->onClicked([]() {
        //     katla::printInfo("Hallo again!");
        // });

        column = column_create();
        // auto column = katla::Column::create();

        column_append_child(column, text);
        // column->updateContainer({
        //         .children = {
        //                 katla::ContainerChild{.child = std::move(label)},
        //                 katla::ContainerChild{.child = std::move(button)},
        //                 katla::ContainerChild{.child = std::move(button2)},
        //         }
        // });

        window_set_child(window, column);

        // window->updateContainer({
        //         .children = {katla::ContainerChild{.child = std::move(column)}}
        // });
}

void on_activate(void* app)
{
    assert(window != NULL);

    window_show(window);
}

int main(int argc, char* argv[])
{
    void* app = 0;
    void* startup_sub = 0, *activate_sub = 0;
    void* window = 0;

    register_gtk_platform();
    
    app = create_application(argc, argv, "cross-application-test");

    startup_sub = application_on_startup(app, on_startup);
    activate_sub = application_on_activate(app, on_activate);

    int result = application_run(app);

    destroy_subscription(startup_sub);
    destroy_subscription(activate_sub);
    destroy_application(app);
}
