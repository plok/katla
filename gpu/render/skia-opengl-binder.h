#ifndef KATLA_SKIA_OPENGL_BINDER_H
#define KATLA_SKIA_OPENGL_BINDER_H

#include "core/SkSurface.h"
#include "gpu/GrContext.h"

// TODO scene / context / canvas etc.... ?

class SkiaOpenGlBinder
{
public:
    SkiaOpenGlBinder();

    void init();

    sk_sp<GrContext> context();
    sk_sp<SkSurface> surface();

private:
    sk_sp<GrContext> _context;
    sk_sp<SkSurface> _surface;

};

#endif