#ifndef KATLA_DIRECT2D_SURFACE_H
#define KATLA_DIRECT2D_SURFACE_H

#include "common/error.h"

#include "../../katla-gpu.h"

#include "windows.h"
#include "d2d1.h"
#include "d2d1_1.h"

struct Direct2D_Private
{
    ID2D1Factory* factory;
};

struct Direct2D_SurfacePrivateDeps
{
    HWND hwnd,
    ID2D1Factory* factory;
};

struct Direct2D_SurfacePrivate
{
    ID2D1HwndRenderTarget* renderTarget;
};

#endif
