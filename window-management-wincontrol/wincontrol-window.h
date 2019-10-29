#ifndef WINCONTROL_WINDOW_H
#define WINCONTROL_WINDOW_H

#include "katla-window-management-wincontrol.h"

#include "window-management/window.h"
#include "common/error.h"

#include "common/subject.h"

#include "app-kit/event-loop-uv.h"

#include "gpu/backend/direct2d/direct2d.h"

#include <memory>
#include <functional>

class RenderView;

class KATLA_WINDOW_MANAGEMENT_WINCONTROL_DECLSPEC WinControlWindow : public Window {
public:
    WinControlWindow(std::shared_ptr<RenderView> renderView,
        std::shared_ptr<WindowProperties> properties,
        HINSTANCE instance, int firstCmdShow);
    virtual ~WinControlWindow();

    void init();

    void show();
    void render();
    void clear();
    void close();

    HWND handle() {
        return m_instance;
    }

    void processEvents();

private:
    static LRESULT CALLBACK WinControlWindow::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    std::shared_ptr<RenderView> _renderView;
    std::shared_ptr<WindowProperties> _properties;


    // TODO graphics-backend
    Direct2D _direct2D;

    HINSTANCE m_appInstance;
    HWND m_instance;

    int m_firstCmdShow;
};

#endif
