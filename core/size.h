#ifndef SIZE_H
#define SIZE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
namespace katla {
#endif

    struct Size_32s {
        int32_t width;
        int32_t height;
    };

    struct Size_32f {
        float width;
        float height;
    };

#ifdef __cplusplus
}
}
#endif

#endif