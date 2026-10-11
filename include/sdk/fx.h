#ifndef SDK_FX_H
#define SDK_FX_H

#include "types.h"
#include "hw_div.h"

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

void MTX_RotX43_(MtxFx43 *mtx, fx32 sin, fx32 cos);
void FX_DivAsync(fx32 numer, fx32 denom);
void FX_InvAsync(fx32 denom);
fx32 FX_GetDivResult(void);
long long Divider_WaitRawQuotient64(void);
void VEC_MultAdd(fx32 a, const VecFx32 *v1, const VecFx32 *v2, VecFx32 *dst);
void VEC_CrossProduct(const VecFx32 *a, const VecFx32 *b, VecFx32 *dst);
fx32 VEC_DotProduct(const VecFx32 *a, const VecFx32 *b);
void VEC_Normalize(const VecFx32 *src, VecFx32 *dst);
void MTX_Identity43(MtxFx43 *mtx);
void Matrix_Expand43Into44(MtxFx43 *src, MtxFx44 *dest);
void Matrix_Take33From44(MtxFx44 *src, MtxFx33 *dest);
void Matrix_MoveAlongLocalAxes43(MtxFx43 *src, MtxFx43 *dest, fx32 x, fx32 y, fx32 z);

#endif
