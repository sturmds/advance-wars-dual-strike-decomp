#ifndef SDK_FX_H
#define SDK_FX_H

#include "types.h"

/* 4 rows of 3: a 3x3 rotation/scale part, then a translation row. */
typedef struct MtxFx43 {
    fx32 m[4][3];
} MtxFx43;

typedef struct VecFx32 {
    fx32 x;
    fx32 y;
    fx32 z;
} VecFx32;

/* 4x4 and 3x3 matrices in row order. */
typedef struct MtxFx44 {
    fx32 m[4][4];
} MtxFx44;

typedef struct MtxFx33 {
    fx32 m[3][3];
} MtxFx33;

/* Hardware divider. DIVCNT mode 1: 64-bit numerator over 32-bit denominator
 * (the denominator register is 64 bits wide). bit 15 of DIVCNT: busy. */
#define REG_DIVCNT ((volatile u16 *)0x04000280)
#define REG_DIV_NUMER ((volatile long long *)0x04000290)
#define REG_DIV_DENOM ((volatile long long *)0x04000298)
#define REG_DIV_RESULT ((volatile long long *)0x040002a0)
#define DIVCNT_MODE_64_32 1
#define DIVCNT_BUSY 0x8000

void MTX_RotX43_(MtxFx43 *mtx, fx32 sin, fx32 cos);
void FX_DivAsync(fx32 numer, fx32 denom);
void FX_InvAsync(fx32 denom);
fx32 FX_GetDivResult(void);
long long Divider_WaitRawQuotient64(void);
void MTX_Identity43(MtxFx43 *mtx);
void Matrix_Expand43Into44(MtxFx43 *src, MtxFx44 *dest);
void Matrix_Take33From44(MtxFx44 *src, MtxFx33 *dest);
void Matrix_MoveAlongLocalAxes43(MtxFx43 *src, MtxFx43 *dest, fx32 x, fx32 y, fx32 z);

#endif
