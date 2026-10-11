#include "types.h"
#include "sdk/gx.h"

void func_0210c06c(int fovSin, int fovCos, int aspect, int nearZ, int farZ, int scale, BOOL load, void *mtx);

/* Same as Gfx3d_SetPerspective (60 degree field of view, 4:3 aspect), but
 * passes mtx on to the SDK call (guess: receives a copy). */
void Gfx3d_LoadPerspectiveProjection(void *mtx)
{
    *REG_MTX_MODE = 0;
    *REG_MTX_IDENTITY = 0;
    func_0210c06c(0x800, 0xddb, 0x1555, 1, 0x2000, 0x1000, TRUE, mtx);
    *REG_MTX_STORE = 0;
}
