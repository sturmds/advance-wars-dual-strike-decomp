#include "types.h"
#include "sdk/gx.h"

void func_0210c06c(int fovSin, int fovCos, int aspect, int nearZ, int farZ, int scale, BOOL load, void *mtx);

/* Sets a 60 degree field of view, 4:3 aspect perspective projection, then
 * stores the matrix in slot 0 of the matrix stack. */
void Gfx3d_SetPerspective(void)
{
    func_0210c06c(0x800, 0xddb, 0x1555, 1, 0x2000, 0x1000, TRUE, NULL);
    *REG_MTX_STORE = 0;
}
