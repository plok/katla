#include "skia-render-view.h"

#include <GLFW/glfw3.h>

#include "gpu/GrContext.h"
#include "gpu/GrRenderTarget.h"
#include "gpu/gl/GrGLInterface.h"

#include "core/SkData.h"
#include "core/SkImage.h"
#include "core/SkStream.h"
#include "core/SkSurface.h"
#include "core/SkCanvas.h"

#include "GrBackendSurface.h"

SkiaRenderView::SkiaRenderView() = default;

void SkiaRenderView::init()
{
    _skia.init();
}

void SkiaRenderView::render()
{
    auto surface = _skia.surface();
    if (!surface) {
        return;
    }

    int width = surface->width();
    int height = surface->height();

    SkCanvas* canvas = surface->getCanvas();
    
    canvas->clear(SK_ColorRED);

    SkPaint paint;
    paint.setColor(SK_ColorWHITE);
    paint.setTextSize(64.0f);

    SkRect rect {10,10, SkIntToScalar(width - 10), SkIntToScalar(height - 10)};
    canvas->drawRect(rect, paint);

    paint.setColor(SK_ColorRED);
    paint.setTextSize(64.0f);

    std::string message = "hoi!";
    canvas->drawText(message.c_str(), strlen(message.c_str()), SkIntToScalar(width/2),
                     SkIntToScalar(height/2), paint);

    canvas->flush();
}

void SkiaRenderView::resize(int /*width*/, int /*height*/)
{
    //render();
}

void SkiaRenderView::cleanup()
{
    // TODO?
}
    