#include "types.h"
#include "sdk/gx.h"

void func_0210bd70(int top, int bottom, int left, int right, int near, int far, int scaleW, int draw, int mtx);   /* SDK orthographic projection (G3i_OrthoW_) */

void Ov004_SetupOrtho3d(void) {
    *REG_DISP3DCNT &= 0xcfef;          /* anti-aliasing off, acknowledge the overflow flags */
    *REG_SWAP_BUFFERS = 3;
    G3X_SetClearColor(0x2bea, 0, 0x7fff, 63, FALSE);
    *REG_VIEWPORT = 0xbfff0000;        /* (0, 0) to (255, 191) */
    func_0210bd70(0, -192, 0, 256, 1, 0x2000, 0x1000, 1, 0);
    *REG_MTX_STORE = 0;
}
