#include "types.h"
#include "sdk/gx.h"

void func_0210c06c(int fovSin, int fovCos, int aspect, int nearZ, int farZ, int scale, BOOL load, void *mtx);

/* Sets the clear colour, viewport and a perspective projection, with
 * anti-aliasing switched on (DISP3DCNT bit 4) and the overflow flags
 * (bits 12 and 13) cleared. */
void Gfx3d_SetupPerspectiveAntialiased(void)
{
    *REG_DISP3DCNT = (*REG_DISP3DCNT & ~0x3000) | 0x10;
    *REG_SWAP_BUFFERS = 1;
    G3X_SetClearColor(0x2bea, 0, 0x7fff, 63, FALSE);
    *REG_VIEWPORT = 0xbfff0000;
    func_0210c06c(0x800, 0xddb, 0x1555, 1, 0x2000, 0x1000, TRUE, NULL);
    *REG_MTX_STORE = 0;
}
