#ifndef SDK_FX_H
#define SDK_FX_H

#include "types.h"

/* 4 rows of 3: a 3x3 rotation/scale part, then a translation row. */
typedef struct MtxFx43 {
    fx32 m[4][3];
} MtxFx43;

void MTX_RotX43_(MtxFx43 *mtx, fx32 sin, fx32 cos);

#endif
