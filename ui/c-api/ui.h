#ifndef KATLA_UI_C_API_UI_H
#define KATLA_UI_C_API_UI_H

#ifdef __cplusplus
extern "C" {
#endif

    struct cSize_32s {
        int width;
        int height;
    };

    void* create_application(int argc, char* argv[], char* appName);
    void destroy_application(void* app);

    void* application_on_startup(void* app, void (*startup_callback)(void* app));
    void* application_on_activate(void* app, void (*activate_callback)(void* app));
    int   application_run(void* app);

    void* application_create_window(void* app, char* title, struct cSize_32s size);

    void destroy_subscription(void* sub);

    void window_show(void* window);
    void window_set_child(void* window_, void* child);

    void* text_create(char* text);

    void* column_create();
    void  column_append_child(void* column, void* child);

    void* button_create();
    void* button_on_clicked(void* button_, void (*on_clicked_callback)(void* cb_button));


#ifdef __cplusplus
}
#endif

#endif