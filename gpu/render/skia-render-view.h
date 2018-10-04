#ifndef KATLA_SKIA_RENDER_VIEW_H
#define KATLA_SKIA_RENDER_VIEW_H

#include "render-view.h"

#include "skia-opengl-binder.h"

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
    SkiaOpenGlBinder _skia;
};

#endif