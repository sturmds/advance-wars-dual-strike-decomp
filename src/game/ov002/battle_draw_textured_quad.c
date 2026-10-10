#include "types.h"

#define GXFIFO_TEXIMAGE_PARAM ((volatile u32 *)0x040004a8)
#define GXFIFO_PLTT_BASE ((volatile u32 *)0x040004ac)

u32 Battle_GetTexHeight(u32 index);
u32 Battle_GetTexWidth(u32 index);
void Gx_SendQuadXY(int x, int y, int z, int width, int height, int u0, int v0, int u1, int v1);

/* Sets the texture and palette, then draws the quad with the texture flipped as asked. The
 * texture sizes are texel counts picked by sizeS and sizeT (0-3: 8 to 64). */
void Battle_DrawTexturedQuad(int x, int y, int z, int width, int height, u32 sizeS, u32 sizeT, u32 texAddr, u32 palAddr, int flipX, int flipY)
{
    u32 texW = Battle_GetTexWidth(sizeS);
    u32 texH = Battle_GetTexHeight(sizeT);

    *GXFIFO_TEXIMAGE_PARAM = ((texAddr << 5) >> 3) | 0x4c000000 | (sizeS << 20) | (sizeT << 23) | 0x20000000;
    *GXFIFO_PLTT_BASE = (palAddr << 5) >> 4;
    if (flipX != 0 && flipY != 0) {
        Gx_SendQuadXY(x, y, z, width, height, texW, texH, 0, 0);
    } else if (flipX != 0) {
        Gx_SendQuadXY(x, y, z, width, height, texW, 0, 0, texH);
    } else if (flipY != 0) {
        Gx_SendQuadXY(x, y, z, width, height, 0, texH, texW, 0);
    } else {
        Gx_SendQuadXY(x, y, z, width, height, 0, 0, texW, texH);
    }
}
