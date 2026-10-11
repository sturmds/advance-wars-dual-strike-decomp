#ifndef SDK_FX_H
#define SDK_FX_H

#include "types.h"
#include "hw_div.h"

/* 64-bit fixed point, for products of two fx32 values before the shift. */
typedef signed long long fx64;

#define FX32_SHIFT 12
#define FX32_HALF 0x800

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
fx32 FX_Div(fx32 numer, fx32 denom);
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

/* Start a division on the divider without setting DIVCNT again: the caller
 * already left it in 64-by-32 mode. Divider_StartFixedQuotient computes numer / denom,
 * Divider_StartFixedReciprocal 1.0 / denom; read either with FX_GetDivResult(64). */
static inline void Divider_StartFixedQuotient(fx32 numer, fx32 denom)
{
    IO_DIVIDER_DIVIDEND = (long long)numer << 32;
    REG_DIV_DENOM = (u32)denom;
}

static inline void Divider_StartFixedReciprocal(fx32 denom)
{
    IO_DIVIDER_DIVIDEND = (long long)0x1000 << 32;
    REG_DIV_DENOM = (u32)denom;
}

/* v32 * v64, where v64 has 32 fraction bits, rounded to fx32. */
static inline fx32 FX_Mul32x64c(fx32 v32, long long v64)
{
    return (fx32)((v64 * v32 + 0x80000000LL) >> 32);
}

/* a * b in fx32, rounded. */
static inline fx32 FX_Mul(fx32 a, fx32 b)
{
    return (fx32)(((long long)a * b + 0x800) >> 12);
}

#endif
