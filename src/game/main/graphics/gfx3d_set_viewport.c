#include "types.h"
#include "sdk/gx.h"

typedef struct Viewport {
  u32 x1;
  u32 y1;
  u32 x2;
  u32 y2;
} Viewport;

static Viewport gFullScreenViewport = {
#include "data/main/gFullScreenViewport.inc.c"
};

/* Sets the 3D viewport to the given corners (0 to 255 and 0 to 191 when
 * viewport is NULL). */
void Gfx3d_SetViewport(const Viewport* viewport) {
  if (viewport == NULL) {
    viewport = &gFullScreenViewport;
  }
  *REG_VIEWPORT = viewport->x1 | viewport->y1 << 8 | viewport->x2 << 16 |
                  viewport->y2 << 24;
}
