#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void Screen_ResetLayerScroll(
    void); /* clears the BG scroll values of the four layers */

/* The register values as plain halfwords: bgCnt[4] is the MOSAIC word that follows BG3CNT. */
#define BG_CNT_RAW(gfx, bg) (*(u16*)&(gfx)->bgCnt[bg])
#define BG_MOSAIC_RAW(gfx) BG_CNT_RAW(gfx, 4)

void Screen_ResetBgRegisters(void) {
  Screen_ResetLayerScroll();
  BG_MOSAIC_RAW(data_027c0280) = 0;
  BG_CNT_RAW(data_027c0280, GFX_LAYER(data_027c0280, 0)->bg) = 0;
  BG_CNT_RAW(data_027c0280, GFX_LAYER(data_027c0280, 1)->bg) = 0;
  BG_CNT_RAW(data_027c0280, GFX_LAYER(data_027c0280, 2)->bg) = 0;
  BG_CNT_RAW(data_027c0280, GFX_LAYER(data_027c0280, 3)->bg) = 0;
}
