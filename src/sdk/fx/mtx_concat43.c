#include "sdk/fx.h"

/* x . y in 64 bits, shifted back to fx32 scale. */
#define DOT3_64(x0, x1, x2, y0, y1, y2) \
  (((fx64)(x0) * (y0) + (fx64)(x1) * (y1) + (fx64)(x2) * (y2)) >> FX32_SHIFT)
#define DOT3(x0, x1, x2, y0, y1, y2) ((fx32)DOT3_64(x0, x1, x2, y0, y1, y2))

/* ab = a * b for 4x3 matrices (row vectors, so the
 * fourth row adds b's translation). ab may be a or b. The columns run back
 * and forth from row to row, so that the b column a row ends with (kept in
 * y0 to y2) starts the next row. */
void MTX_Concat43(const MtxFx43* a, const MtxFx43* b, MtxFx43* ab) {
  MtxFx43 tmp;
  MtxFx43* out;
  fx32 x0, x1, x2;
  fx32 y0, y1, y2;

  out = (ab == b) ? &tmp : ab;

  x0 = a->m[0][0];
  x1 = a->m[0][1];
  x2 = a->m[0][2];
  out->m[0][0] = DOT3(x0, x1, x2, b->m[0][0], b->m[1][0], b->m[2][0]);
  out->m[0][1] = DOT3(x0, x1, x2, b->m[0][1], b->m[1][1], b->m[2][1]);
  y0 = b->m[0][2];
  y1 = b->m[1][2];
  y2 = b->m[2][2];
  out->m[0][2] = DOT3(x0, x1, x2, y0, y1, y2);

  x0 = a->m[1][0];
  x1 = a->m[1][1];
  x2 = a->m[1][2];
  out->m[1][2] = DOT3(x0, x1, x2, y0, y1, y2);
  out->m[1][1] = DOT3(x0, x1, x2, b->m[0][1], b->m[1][1], b->m[2][1]);
  y0 = b->m[0][0];
  y1 = b->m[1][0];
  y2 = b->m[2][0];
  out->m[1][0] = DOT3(x0, x1, x2, y0, y1, y2);

  x0 = a->m[2][0];
  x1 = a->m[2][1];
  x2 = a->m[2][2];
  out->m[2][0] = DOT3(x0, x1, x2, y0, y1, y2);
  out->m[2][1] = DOT3(x0, x1, x2, b->m[0][1], b->m[1][1], b->m[2][1]);
  y0 = b->m[0][2];
  y1 = b->m[1][2];
  y2 = b->m[2][2];
  out->m[2][2] = DOT3(x0, x1, x2, y0, y1, y2);

  x0 = a->m[3][0];
  x1 = a->m[3][1];
  x2 = a->m[3][2];
  out->m[3][2] = (fx32)(b->m[3][2] + DOT3_64(x0, x1, x2, y0, y1, y2));
  out->m[3][1] = (fx32)(b->m[3][1] + DOT3_64(x0, x1, x2, b->m[0][1], b->m[1][1],
                                             b->m[2][1]));
  out->m[3][0] = (fx32)(b->m[3][0] + DOT3_64(x0, x1, x2, b->m[0][0], b->m[1][0],
                                             b->m[2][0]));

  if (out == &tmp) {
    *ab = tmp;
  }
}
