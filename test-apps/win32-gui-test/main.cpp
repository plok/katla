#ifndef UNICODE
#define UNICODE
#endif 

#include "window-management-wincontrol/wincontrol-window.h"

#include "graphics/render-view.h"
#include "app-kit/core-application.h"

#include "common/size.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    auto renderView = std::make_shared<RenderView>();
    auto windowProperties = std::make_shared<WindowProperties>();
    windowProperties->size = Size_32s {800, 600};

    WinControlWindow window (renderView, windowProperties, hInstance, nCmdShow);
    window.init();
    window.show();

    GraphicsConfiguration conf;

    Direct2D direct2d;
    direct2d.init(conf);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

