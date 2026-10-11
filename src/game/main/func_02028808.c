#include "types.h"
#include "sdk/gx.h"

void G3i_OrthoW_(int top, int bottom, int left, int right, int near, int far, int scaleW, int draw, int mtx);

void func_02028808(void) {
    *REG_DISP3DCNT &= 0xcfef;           
    *REG_SWAP_BUFFERS = 1;
    G3X_SetClearColor(0x2bea, 0, 0x7fff, 63, FALSE);
    *REG_VIEWPORT = 0xbfff0000;         
    G3i_OrthoW_(0, -192, 0, 256, 1, 0x2000, 0x1000, 1, 0);
    *REG_MTX_STORE = 0;
}
