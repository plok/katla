#ifndef KATLA_CORE_EVENT_LOOP_H
#define KATLA_CORE_EVENT_LOOP_H

#include "katla/core/error.h"

namespace katla {

class EventLoop {
  public:
    virtual ~EventLoop() = default;

    virtual katla::result<void, Error> init() = 0;
    virtual katla::result<void, Error> run() = 0;
    virtual katla::result<void, Error> close() = 0;

  private:
};

}

#endif
