#include "direct2d-surface.h"
#include "direct2d-private.h"

#include <cstring>
#include <iostream>
#include <vector>

Direct2D_Surface::Direct2D_Surface(const Direct2D_SurfacePrivateDeps& deps) :
    _deps(deps)
{
    _d = std::make_unique<Direct2D_SurfacePrivate>();
}

Direct2D_Surface::~Direct2D_Surface()
{
}

// TODO no private in structure, use more like c-api style
Direct2D_Surface Direct2D_Surface::create(const Direct2D_SurfacePrivateDeps& deps)
{
    // Obtain the size of the drawing area.
    RECT rc;
    GetClientRect(deps.hwnd, &rc);

    // Create a Direct2D render target
    ID2D1HwndRenderTarget* pRT = NULL;
    HRESULT hr = deps->factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            deps.hwnd,
            D2D1::SizeU(
                rc.right - rc.left,
                rc.bottom - rc.top)
        ),
        &pRT
    );
}

std::unique_ptr<WindowFactory> Direct2D::windowFactory()
{
    // return std::make_unique<Direct2DWindowFactory>(_openGlConfiguration);
    return std::unique_ptr<WindowFactory>();
}
