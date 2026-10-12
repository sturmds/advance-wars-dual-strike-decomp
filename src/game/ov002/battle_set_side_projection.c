#include "types.h"
#include "sdk/gx.h"

BOOL Battle_IsSideAir(int side);
/* Orthographic projection (a guess from the arguments): top, bottom, left, right, near, far, scale. */
void Projection_BuildPerspectiveW(int top, int bottom, int left, int right,
                                  int nearZ, int farZ, int scale, BOOL load,
                                  void* mtx);

/* Sets the projection for one side's view: the screen half it draws on and, for air units, a
 * taller field. */
void Battle_SetSideProjection(int side) {
  *REG_MTX_MODE = 0;
  if (side == 0) {
    if (Battle_IsSideAir(side) == 1) {
      Projection_BuildPerspectiveW(0, -6, -10, -2, 8, 0x1000, 0x1000, TRUE,
                                   NULL);
    } else {
      Projection_BuildPerspectiveW(3, -3, -10, -2, 8, 0x1000, 0x1000, TRUE,
                                   NULL);
    }
  } else {
    if (Battle_IsSideAir(side) == 1) {
      Projection_BuildPerspectiveW(0, -6, 2, 10, 8, 0x1000, 0x1000, TRUE, NULL);
    } else {
      Projection_BuildPerspectiveW(3, -3, 2, 10, 8, 0x1000, 0x1000, TRUE, NULL);
    }
  }
  *REG_MTX_STORE = 0;
  *REG_MTX_MODE = 2;
}
