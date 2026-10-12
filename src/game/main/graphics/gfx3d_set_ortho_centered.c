#include "types.h"
#include "sdk/gx.h"

void G3i_OrthoW_(int top, int bottom, int left, int right, int near, int far,
                 int scaleW, int draw, int mtx);

/* Sets an orthographic projection of 256 x 192 units around the origin
 * (left -128, right 128, top 96, bottom -96), then stores the matrix in slot 0
 * of the matrix stack. */
void Gfx3d_SetOrthoCentered(void) {
  G3i_OrthoW_(96, -96, -128, 128, 1, 0x2000, 0x1000, 1, 0);
  *REG_MTX_STORE = 0;
}
