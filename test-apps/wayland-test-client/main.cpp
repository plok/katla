#include "app-kit/core-application.h"

#include "compositor/wayland/client/wayland-display.h"

#include "graphics/image.h"
#include "graphics/simple-painter.h"

#include "string.h"
#include <iostream>
#include <vector>


int main(int argc, char* argv[])
{
    bool useOpenGL = false;

    std::vector<std::string> args;
    for (int i=0; i<argc; i++) {
        std::string arg = argv[i];
        args.push_back(arg);
    }

    std::cout << "Starting wayland-test-client with args: "; for(auto s : args) std::cout << s; std::cout << std::endl;

    CoreApplication app;
    auto appError = app.init();
    if (appError) {
        std::cout << appError->name << " " << appError->message << std::endl << std::flush;
        return EXIT_FAILURE;
    }
	
    WaylandDisplay display;
    display.init();

    auto surface = display.createSurface();
    auto buffer = surface->buffer();

    Size_32s size { 640, 480 };
    Image image(buffer->data(), size, 4, 640*4, 8);
    SimplePainter painter;
    painter.drawRectangle(image, Rect_32s {100, 100, 200, 200}, Color_8u_ARGB {0,255,0,0});

    // TODO use eventloop of appKit
    bool done = false;
    while (!done) {
        if (wl_display_dispatch(display.handle()) < 0) {
            std::cerr << "Main loop error" << std::endl;
            done = true;
        }
    }

    appError = app.run();
    if (appError) {
        std::cout << appError->name << " " << appError->message << std::endl;
        return -1;
    }

    return 0;
}
