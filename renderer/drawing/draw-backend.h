#ifndef KATLA_DRAW_BACKEND_H
#define KATLA_DRAW_BACKEND_H

class DrawBackend
{
public:
    DrawBackend() = default;
    virtual ~DrawBackend() = default;

    virtual void init() = 0;
    virtual void cleanup() = 0;

    virtual void draw() = 0;
};

#endif