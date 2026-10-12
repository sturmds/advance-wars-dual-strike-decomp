#include "types.h"

void func_0200c38c(s32* out, int x, int y, int z);
void Mtx44_ProjectVec(s32* vec, void* mtx,
                      s32* out); /* vector times 4x3 matrix */

void func_0200c3d8(int x, int y, int z, void* mtx, s32* out) {
  s32 vec[3] = {0, 0, 0};

  func_0200c38c(vec, x, y, z);
  Mtx44_ProjectVec(vec, mtx, vec);
  out[0] = vec[0];
  out[1] = (vec[1] * 192) / 256;
  out[2] = 0;
}
