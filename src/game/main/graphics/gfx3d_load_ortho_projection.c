#include "types.h"
#include "sdk/gx.h"

void G3i_OrthoW_(int top, int bottom, int left, int right, int near, int far, int scaleW, int draw, void *mtx);

/* Loads an orthographic projection (x -0x1000 to 0x1000, y -0xc00 to 0xc00,
 * z 0 to 0x2800) over an identity matrix and stores it in slot 0 of the
 * matrix stack. mtx is passed on to the SDK call (guess: receives a copy). */
void Gfx3d_LoadOrthoProjection(void *mtx)
{
    *REG_MTX_MODE = 0;
    *REG_MTX_IDENTITY = 0;
    G3i_OrthoW_(0xc00, -0xc00, -0x1000, 0x1000, 0, 0x2800, 0x1000, 1, mtx);
    *REG_MTX_STORE = 0;
}
