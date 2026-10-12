#include "types.h"
#include "sdk/gx.h"

void G3i_PerspectiveW_(int fovSin, int fovCos, int aspect, int nearZ, int farZ,
                       int scale, BOOL load,
                       void* mtx); /* SDK perspective projection */

void Ov004_SetupPerspective3d(void) {
  *REG_DISP3DCNT &=
      0xcfef; /* anti-aliasing off, acknowledge the overflow flags */
  *REG_SWAP_BUFFERS = 3;
  G3X_SetClearColor(0x2bea, 0, 0x7fff, 63, FALSE);
  *REG_VIEWPORT = 0xbfff0000; /* (0, 0) to (255, 191) */
  G3i_PerspectiveW_(0x800, 0xddb, 0x1555, 1, 0x2000, 0x1000, TRUE, NULL);
  *REG_MTX_STORE = 0;
}
