//
// Created by simon on 10/15/18.
//

#include "render-stats.h"

// TODO does this leak?
using Clock = std::chrono::high_resolution_clock;

void RenderStats::beginFrame() {
    _lastBegin = Clock::now();
}

void RenderStats::endFrame() {
    auto now = Clock::now();
    if (_lastRender) {
        _lastRenderInterval = std::chrono::duration_cast<std::chrono::microseconds>(now - _lastRender.value()).count();
    }

    _lastRender = now;
    if (_lastBegin) {
        _lastDuration = std::chrono::duration_cast<std::chrono::microseconds>(_lastRender.value() - _lastBegin.value()).count();
    }
}

RenderStats::Stats RenderStats::stats()
{
    Stats stats (_lastDuration, _lastRenderInterval);
    return stats;
}