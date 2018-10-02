#ifndef KATLA_SKIA_RENDER_VIEW_H
#define KATLA_SKIA_RENDER_VIEW_H

#include "render-view.h"

#include "core/SkSurface.h"
#include "gpu/GrContext.h"

// TODO scene / context / canvas etc.... ?

class SkiaRenderView : public RenderView
{
public:
    SkiaRenderView();

    void init();
    void render();
    void cleanup();

    void resize(int width, int height);

private:
    sk_sp<GrContext> m_context;
    sk_sp<SkSurface> m_surface;

};

#endif