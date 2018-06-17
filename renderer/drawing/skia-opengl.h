#ifndef KATLA_SKIA_H
#define KATLA_SKIA_H

#include "draw-backend.h"

#include "core/SkSurface.h"
#include "gpu/GrContext.h"

class SkiaOpenGL : public DrawBackend
{
public:
    SkiaOpenGL();

    void init();
    void draw();
    void cleanup();

private:
    sk_sp<GrContext> m_context;
    sk_sp<SkSurface> m_surface;

};

#endif