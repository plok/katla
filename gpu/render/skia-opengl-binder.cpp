#include "skia-opengl-binder.h"

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

SkiaOpenGlBinder::SkiaOpenGlBinder() = default;

void SkiaOpenGlBinder::init()
{
    auto interface = GrGLMakeNativeInterface();
    
    _context = GrContext::MakeGL(interface);
    
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
    

    _surface = sk_sp<SkSurface>(
        SkSurface::MakeFromBackendRenderTarget(_context.get(),
                                            target,
                                               kBottomLeft_GrSurfaceOrigin,
                                               colorType,
                                               nullptr,
                                               &props));
    if (!_surface) {
        SkDebugf("SkSurface::MakeRenderTarget returned null\n");
        return;
    }
}

sk_sp<GrContext> SkiaOpenGlBinder::context()
{
    return _context;
}
sk_sp<SkSurface> SkiaOpenGlBinder::surface()
{
    return _surface;
}
