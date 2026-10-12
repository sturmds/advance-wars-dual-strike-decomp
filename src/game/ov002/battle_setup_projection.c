#include "types.h"
#include "sdk/gx.h"

/* Looks like the SDK's Projection_BuildOrthoW: top, bottom, left, right, near, far, scale, a flag, a matrix pointer. */
void Projection_BuildPerspectiveW(int top, int bottom, int left, int right,
                                  int near, int far, int scale, int flag,
                                  void* mtx);

/* Matrix mode 0 (projection): sets a projection (probably orthographic) and stores it in
 * matrix stack slot 0 (MTX_STORE), then switches to mode 2. */
void Battle_SetupProjection(void) {
  *REG_MTX_MODE = 0;
  Projection_BuildPerspectiveW(3, -3, -4, 4, 8, 0x1000, 0x1000, 1, NULL);
  *REG_MTX_STORE = 0;
  *REG_MTX_MODE = 2;
}
