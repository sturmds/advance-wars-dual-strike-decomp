#include "sdk/fx.h"

#define DOT4(x0, x1, x2, x3, y0, y1, y2, y3) \
    ((fx32)(((fx64)(x0) * (y0) + (fx64)(x1) * (y1) + (fx64)(x2) * (y2) + (fx64)(x3) * (y3)) >> FX32_SHIFT))

/* ab = a * b for 4x4 matrices. ab may be a or b. As in
 * MTX_Concat43, the column a row ends with (kept in y0 to y3) starts the
 * next row. */
void MTX_Concat44(const MtxFx44 *a, const MtxFx44 *b, MtxFx44 *ab)
{
    MtxFx44 tmp;
    MtxFx44 *out;
    fx32 x0, x1, x2, x3;
    fx32 y0, y1, y2, y3;

    out = (ab == b) ? &tmp : ab;

    x0 = a->m[0][0];
    x1 = a->m[0][1];
    x2 = a->m[0][2];
    x3 = a->m[0][3];
    out->m[0][0] = DOT4(x0, x1, x2, x3, b->m[0][0], b->m[1][0], b->m[2][0], b->m[3][0]);
    out->m[0][1] = DOT4(x0, x1, x2, x3, b->m[0][1], b->m[1][1], b->m[2][1], b->m[3][1]);
    out->m[0][3] = DOT4(x0, x1, x2, x3, b->m[0][3], b->m[1][3], b->m[2][3], b->m[3][3]);
    y0 = b->m[0][2];
    y1 = b->m[1][2];
    y2 = b->m[2][2];
    y3 = b->m[3][2];
    out->m[0][2] = DOT4(x0, x1, x2, x3, y0, y1, y2, y3);

    x0 = a->m[1][0];
    x1 = a->m[1][1];
    x2 = a->m[1][2];
    x3 = a->m[1][3];
    out->m[1][2] = DOT4(x0, x1, x2, x3, y0, y1, y2, y3);
    out->m[1][1] = DOT4(x0, x1, x2, x3, b->m[0][1], b->m[1][1], b->m[2][1], b->m[3][1]);
    out->m[1][3] = DOT4(x0, x1, x2, x3, b->m[0][3], b->m[1][3], b->m[2][3], b->m[3][3]);
    y0 = b->m[0][0];
    y1 = b->m[1][0];
    y2 = b->m[2][0];
    y3 = b->m[3][0];
    out->m[1][0] = DOT4(x0, x1, x2, x3, y0, y1, y2, y3);

    x0 = a->m[2][0];
    x1 = a->m[2][1];
    x2 = a->m[2][2];
    x3 = a->m[2][3];
    out->m[2][0] = DOT4(x0, x1, x2, x3, y0, y1, y2, y3);
    out->m[2][1] = DOT4(x0, x1, x2, x3, b->m[0][1], b->m[1][1], b->m[2][1], b->m[3][1]);
    out->m[2][3] = DOT4(x0, x1, x2, x3, b->m[0][3], b->m[1][3], b->m[2][3], b->m[3][3]);
    y0 = b->m[0][2];
    y1 = b->m[1][2];
    y2 = b->m[2][2];
    y3 = b->m[3][2];
    out->m[2][2] = DOT4(x0, x1, x2, x3, y0, y1, y2, y3);

    x0 = a->m[3][0];
    x1 = a->m[3][1];
    x2 = a->m[3][2];
    x3 = a->m[3][3];
    out->m[3][2] = DOT4(x0, x1, x2, x3, y0, y1, y2, y3);
    out->m[3][1] = DOT4(x0, x1, x2, x3, b->m[0][1], b->m[1][1], b->m[2][1], b->m[3][1]);
    out->m[3][0] = DOT4(x0, x1, x2, x3, b->m[0][0], b->m[1][0], b->m[2][0], b->m[3][0]);
    out->m[3][3] = DOT4(x0, x1, x2, x3, b->m[0][3], b->m[1][3], b->m[2][3], b->m[3][3]);
    if (out == &tmp) {
        *ab = tmp;
    }
}
