#include "skia-opengl.h"

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

SkiaOpenGL::SkiaOpenGL()
{
}

void SkiaOpenGL::init()
{
    auto interface = GrGLMakeNativeInterface();
    
    m_context = GrContext::MakeGL(interface);
    
    int width = 800;
    int height = 600;

    // Get the current draw framebuffer 
    GLint drawFboId = 0;
    glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &drawFboId);


    GrGLFramebufferInfo info;
    info.fFBOID = (GrGLuint) drawFboId;
    

    // TODO: detect!
    info.fFormat = GL_RGBA8;
    SkColorType colorType = kRGBA_8888_SkColorType;
    ///

    // Create render target
    GrBackendRenderTarget target(width, height, /*kMsaaSampleCount*/ 0, /*kStencilBits*/ 8, info);


    SkSurfaceProps props(SkSurfaceProps::kLegacyFontHost_InitType);
    

    m_surface = sk_sp<SkSurface>(
        SkSurface::MakeFromBackendRenderTarget(m_context.get(),
                                            target,
                                               kBottomLeft_GrSurfaceOrigin,
                                               colorType,
                                               nullptr,
                                               &props));
    if (!m_surface) {
        SkDebugf("SkSurface::MakeRenderTarget returned null\n");
        return;
    }
}

void SkiaOpenGL::draw()
{
    if (!m_surface) {
        return;
    }

    int width = m_surface->width();
    int height = m_surface->height();

    SkCanvas* canvas = m_surface->getCanvas();
    
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

void SkiaOpenGL::cleanup()
{
    // TODO?
}
    