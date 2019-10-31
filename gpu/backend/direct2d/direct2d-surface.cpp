#include "direct2d-surface.h"
#include "direct2d-private.h"

#include <cstring>
#include <iostream>
#include <vector>

Direct2D_Surface::Direct2D_Surface(const Direct2D_SurfacePrivateDeps& deps, std::unique_ptr<Direct2D_SurfacePrivate> p) :
    _deps(deps),
    _d(std::move(p))
{
}

Direct2D_Surface::~Direct2D_Surface()
{
}

// TODO no private in structure, use more like c-api style
std::unique_ptr<Direct2D_Surface> Direct2D_Surface::create(const Direct2D_SurfacePrivateDeps& deps)
{
    // Obtain the size of the drawing area.
    RECT rc;
    GetClientRect(deps.hwnd, &rc);

    // Create a Direct2D render target
    ID2D1HwndRenderTarget* pRT = NULL;
    HRESULT hr = deps.factory->CreateHwndRenderTarget(
        D2D1::RenderTargetProperties(),
        D2D1::HwndRenderTargetProperties(
            deps.hwnd,
            D2D1::SizeU(
                rc.right - rc.left,
                rc.bottom - rc.top)
        ),
        &pRT
    );

    auto surfacePrivate = std::make_unique<Direct2D_SurfacePrivate>();
    surfacePrivate->renderTarget = pRT;

    auto surface = std::make_unique<Direct2D_Surface>(deps, std::move(surfacePrivate));

    // TODO test std::move
    return std::move(surface);
}
