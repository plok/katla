#ifndef KATLA_RENDER_VIEW_H
#define KATLA_RENDER_VIEW_H

class RenderView
{
public:
    RenderView() = default;
    virtual ~RenderView() = default;

    virtual void init() = 0;
    virtual void cleanup() = 0;

    virtual void resize(int width, int height) = 0;

    virtual void render() = 0;
};

#endif