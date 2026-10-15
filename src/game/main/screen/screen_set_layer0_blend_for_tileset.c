#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen.h"

/* Like Screen_SetLayer0Additive, but blends layer 0 over layers 2 and 3,
 * the sprites and the backdrop at EVA 10, EVB 8 on tileset 1 and at EVA 6,
 * EVB 16 on the others. Bit 7 of the tileset byte is not part of the
 * tileset. A screen config record (data_02169f48) points to it. */
void Screen_SetLayer0BlendForTileset(void) {
  u16 eva;
  u16 evb;

  Screen_SetLayer1Front();
  if ((data_027c027c->unk_48[data_027c0284]->unk_1c4->tileset & 0x7f) == 1) {
    eva = 10;
    evb = 8;
  } else {
    eva = 6;
    evb = 16;
  }
  data_027c0280->blendMode = 1;
  data_027c0280->blendEva = eva;
  data_027c0280->blendEvb = evb;
  data_027c0280->blendEvy = 0;
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendTopBackdrop = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 1;
  data_027c0280->blendBottomBackdrop = 1;
  Screen_ResetWindows();
}
