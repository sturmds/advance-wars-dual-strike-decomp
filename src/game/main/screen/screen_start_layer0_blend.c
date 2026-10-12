#include "types.h"
#include "game/screen_other_map.h"

/* Only the field this function clears is known. */
typedef struct Unk_020e47cc {
  u8 pad_00[0x64];
  u16 unk_64;
} Unk_020e47cc;

/* Starts an alpha blend of layer 0 over layers 1 to 3 and the sprites with
 * EVA 0 and EVB 16, so layer 0 begins fully transparent. */
void Screen_StartLayer0Blend(Unk_020e47cc* owner) {
  data_027c0280->blendTopBg[data_027c0280->layer0Bg] = 1;
  data_027c0280->blendTopBg[data_027c0280->layer1Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer2Bg] = 0;
  data_027c0280->blendTopBg[data_027c0280->layer3Bg] = 0;
  data_027c0280->blendTopObj = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer0Bg] = 0;
  data_027c0280->blendBottomBg[data_027c0280->layer1Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer2Bg] = 1;
  data_027c0280->blendBottomBg[data_027c0280->layer3Bg] = 1;
  data_027c0280->blendBottomObj = 1;
  data_027c0280->blendMode = 1;
  data_027c0280->blendEva = 0;
  data_027c0280->blendEvb = 16;
  data_027c0280->blendEvy = 0;
  owner->unk_64 = 0;
}
