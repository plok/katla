#include "wincontrol-window.h"

#include "graphics/render-view.h"
#include "graphics/graphics-configuration.h"

#include "Windows.h"

#include <iostream>
#include <utility>
#include <string>
#include <sstream>

WinControlWindow::WinControlWindow(
        std::shared_ptr<RenderView> renderView, 
        std::shared_ptr<WindowProperties> properties,
        HINSTANCE appInstance,
        int firstCmdShow) :
    _renderView(std::move(renderView)),
    _properties(std::move(properties)),
    m_appInstance(appInstance),
    m_firstCmdShow(firstCmdShow)
{
    std::cout << "contruct" << std::endl;
}

WinControlWindow::~WinControlWindow()
{
}

void WinControlWindow::init()
{
    // Prereq
    GraphicsConfiguration configuration = {};
    configuration.graphicsBackend = GraphicsBackends::Direct2D; // TODO remove

    _direct2D.init(configuration);

    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"WinControlWindow";
    
    WNDCLASSEXW wc = { };
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = m_appInstance;
    wc.lpszClassName = CLASS_NAME;

    // TODO check if already exists? register only once
    // TODO check return
    auto result = RegisterClassExW(&wc);

    std::cout << result << std::endl;

    // Create the window.
    m_instance = CreateWindowExW(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        m_appInstance,  // Instance handle
        this        // Additional application data
        );

    if (m_instance == NULL)
    {
        return; // TODO
    }
}

void WinControlWindow::show()
{
    ShowWindow(m_instance, m_firstCmdShow);

    int width = 0, height = 0;
    // glfwGetFramebufferSize(_window, &width, &height);

    // glViewport(0, 0, width, height);

    clear();
    render();
}

void WinControlWindow::clear()
{
    // if (!_window) {
    //     return;
    // }

    // glClearColor(1, 1, 1, 1);
    // glClearStencil(0);
    // glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void WinControlWindow::close()
{
    // if (!_window) {
    //     return;
    // }

    // _closeRequested = true;
    // // TODO do not call from callback
    // glfwDestroyWindow (_window);
}

void WinControlWindow::render()
{

    // if (!_window || _closeRequested) {
    //     return;
    // }
    
    // if (_renderView) {
    //     _renderView->render();
    // }

    // glfwSwapBuffers(_window);
    // // glfwPollEvents();
}  

void WinControlWindow::processEvents()
{
    // glfwPollEvents();
}

// void WcWindow::windowSizeCallback(GLFWwindow *window, int width, int height)
// {    
//     auto userPtr = glfwGetWindowUserPointer(window);
//     if (!userPtr) {
//         return;
//     }

//     auto windowObj = static_cast<GlfwWindow*>(userPtr);

//     windowObj->windowSizeChanged(width, height);
// }

// void GlfwWindow::windowSizeChanged(int width, int height)
// {
//     // std::cout << "size" << std::endl;

//     // TODO schedule, not in callback
//     if (_renderView) {
//         _renderView->resize(width, height);
//     }
//     // TODO needed, not in callback?
//     //render();

//     // if (_events->sizeChanged) {

//     //     Size size {width, height};
//     //     _events->sizeChanged->publish(size);
//     // }
// }

// void GlfwWindow::windowFocusCallback(GLFWwindow * /*window*/)
// {
//     std::cout << "focus" << std::endl;
// }

// void GlfwWindow::windowCloseCallback(GLFWwindow *window)
// {
//     auto userPtr = glfwGetWindowUserPointer(window);
//     if (!userPtr) {
//         return;
//     }

//     auto windowObj = static_cast<GlfwWindow*>(userPtr);

//     windowObj->windowCloseRequested();
// }

// void GlfwWindow::windowCloseRequested()
// {
//     _closeRequested = false;

//     if (_properties->closeRequested) {
//         _properties->closeRequested->publish(true);
//     }
// }

// void GlfwWindow::framebufferSizeCallback(GLFWwindow* window, int width, int height)
// {
//     // std::cout << "fb size" << std::endl;

//     auto userPtr = glfwGetWindowUserPointer(window);
//     if (!userPtr) {
//         return;
//     }
//     auto windowObj = static_cast<GlfwWindow*>(userPtr);

//     windowObj->framebufferSize(width, height);
// }

// void GlfwWindow::framebufferSize(int width, int height)
// {
//     glViewport(0, 0, width, height);
//     render();
// }

// LRESULT WinControlWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WinControlWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    WinControlWindow* window = reinterpret_cast<WinControlWindow*>(GetWindowLongPtr( hwnd, GWLP_USERDATA ));

    // std::cout << uMsg << std::endl;

    std::stringstream ss;
    ss << uMsg;
    OutputDebugString(ss.str().c_str());

    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);



            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
