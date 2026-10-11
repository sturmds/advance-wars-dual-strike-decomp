#include "types.h"
#include "sdk/gx.h"

/* Turns on each VRAM bank whose bit is set in banks (bit 0 = A ... bit 8 = I). */
void Vram_PowerOnBanks(u32 banks)
{
    if (banks & GX_VRAM_A) {
        *REG_VRAMCNT_A = VRAMCNT_ENABLE;
    }
    if (banks & GX_VRAM_B) {
        *REG_VRAMCNT_B = VRAMCNT_ENABLE;
    }
    if (banks & GX_VRAM_C) {
        *REG_VRAMCNT_C = VRAMCNT_ENABLE;
    }
    if (banks & GX_VRAM_D) {
        *REG_VRAMCNT_D = VRAMCNT_ENABLE;
    }
    if (banks & GX_VRAM_E) {
        *REG_VRAMCNT_E = VRAMCNT_ENABLE;
    }
    if (banks & GX_VRAM_F) {
        *REG_VRAMCNT_F = VRAMCNT_ENABLE;
    }
    if (banks & GX_VRAM_G) {
        *REG_VRAMCNT_G = VRAMCNT_ENABLE;
    }
    if (banks & GX_VRAM_H) {
        *REG_VRAMCNT_H = VRAMCNT_ENABLE;
    }
    if (banks & GX_VRAM_I) {
        *REG_VRAMCNT_I = VRAMCNT_ENABLE;
    }
}
