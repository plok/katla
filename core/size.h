#ifndef SIZE_H
#define SIZE_H

#include <stdint.h>

namespace katla {

    struct Size_32s {
        int32_t width;
        int32_t height;
    };

    struct Size_32f {
        float width;
        float height;
    };

}

#endif