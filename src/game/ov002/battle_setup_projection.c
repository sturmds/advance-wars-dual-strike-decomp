#include "types.h"

/* Looks like the SDK's Projection_BuildOrthoW: top, bottom, left, right, near, far, scale, a flag, a matrix pointer. */
void func_0210c25c(int top, int bottom, int left, int right, int near, int far, int scale, int flag, void *mtx);

/* Matrix mode 0 (projection): sets a projection (probably orthographic) and stores it in
 * matrix stack slot 0 (MTX_STORE), then switches to mode 2. */
void Battle_SetupProjection(void)
{
    *(volatile int *)0x4000440 = 0;
    func_0210c25c(3, -3, -4, 4, 8, 0x1000, 0x1000, 1, NULL);
    *(volatile int *)0x400044c = 0;
    *(volatile int *)0x4000440 = 2;
}
