#include "types.h"
#include "sdk/fx.h"

/* Multiplies vec by the 4x4 matrix mtx (vec's w taken as 1.0) and divides
 * x, y and z by the resulting w. out may be vec. */
void Mtx44_ProjectVec(VecFx32* vec, MtxFx44* mtx, VecFx32* out) {
  fx32 x = vec->x;
  fx32 y = vec->y;
  fx32 z = vec->z;
  fx32 w;

  out->x = (fx32)(((long long)x * mtx->m[0][0] + (long long)y * mtx->m[1][0] +
                   (long long)z * mtx->m[2][0]) >>
                  12);
  out->x += mtx->m[3][0];
  out->y = (fx32)(((long long)x * mtx->m[0][1] + (long long)y * mtx->m[1][1] +
                   (long long)z * mtx->m[2][1]) >>
                  12);
  out->y += mtx->m[3][1];
  out->z = (fx32)(((long long)x * mtx->m[0][2] + (long long)y * mtx->m[1][2] +
                   (long long)z * mtx->m[2][2]) >>
                  12);
  out->z += mtx->m[3][2];
  w = (fx32)(((long long)x * mtx->m[0][3] + (long long)y * mtx->m[1][3] +
              (long long)z * mtx->m[2][3]) >>
             12);
  w += mtx->m[3][3];
  out->x = ((long long)out->x << 12) / w;
  out->y = ((long long)out->y << 12) / w;
  out->z = ((long long)out->z << 12) / w;
}
