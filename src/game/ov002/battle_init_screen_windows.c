#include "types.h"
#include "sdk/gx.h"
#include "game/Unk_02088d04.h"

void Screen_SetBgMode(int bgMode);
void Screen_InitLayers(const GfxLayer* config);

extern Unk_027c0280* data_027c0280;
extern GfxLayer Ov002_gBattleLayerConfig[4];

/* Sets up the battle screen's layers and two windows: window 0 covers the
 * left half (x 0-125) and shows layer 2, window 1 the right half (x 129-254)
 * and shows layer 3. Layers 0 and 1 and the sprites show in both. */
void Battle_InitScreenWindows(void) {
  Screen_SetBgMode(0);
  Screen_InitLayers(Ov002_gBattleLayerConfig);
  *REG_DISP3DCNT = (*REG_DISP3DCNT & ~0x3000) |
                   0x10; /* acknowledge the overflow flags, anti-aliasing on */
  G3X_SetClearColor(0x2bea, 0, 0x7fff, 63, FALSE);
  *REG_DISPCNT &= ~0x07000000;     /* BG character base offset 0 */
  *REG_DISPCNT &= ~0x38000000;     /* BG screen base offset 0 */
  data_027c0280->dispCnt |= 0x100; /* BG0-BG3 and OBJ on */
  data_027c0280->dispCnt |= 0x200;
  data_027c0280->dispCnt |= 0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  data_027c0280->bgCnt[data_027c0280->layer0Bg].priority = 1;
  data_027c0280->bgCnt[data_027c0280->layer1Bg].priority = 0;
  data_027c0280->bgCnt[data_027c0280->layer2Bg].priority = 2;
  data_027c0280->bgCnt[data_027c0280->layer3Bg].priority = 3;
  data_027c0280->dispCnt |= 0x2000; /* windows 0 and 1 on, OBJ window off */
  data_027c0280->dispCnt |= 0x4000;
  data_027c0280->dispCnt &= ~0x8000;
  data_027c0280->win0Left = 0;
  data_027c0280->win0Top = 0;
  data_027c0280->win0Right = 126;
  data_027c0280->win0Bottom = 192;
  data_027c0280->win1Left = 129;
  data_027c0280->win1Top = 0;
  data_027c0280->win1Right = 255;
  data_027c0280->win1Bottom = 192;
  data_027c0280->win0In.bg[data_027c0280->layer0Bg] = 1;
  data_027c0280->win0In.bg[data_027c0280->layer1Bg] = 1;
  data_027c0280->win0In.bg[data_027c0280->layer2Bg] = 1;
  data_027c0280->win0In.bg[data_027c0280->layer3Bg] = 0;
  data_027c0280->win0In.obj = 1;
  data_027c0280->win1In.bg[data_027c0280->layer0Bg] = 1;
  data_027c0280->win1In.bg[data_027c0280->layer1Bg] = 1;
  data_027c0280->win1In.bg[data_027c0280->layer2Bg] = 0;
  data_027c0280->win1In.bg[data_027c0280->layer3Bg] = 1;
  data_027c0280->win1In.obj = 1;
  data_027c0280->winOut.bg[data_027c0280->layer0Bg] = 0;
  data_027c0280->winOut.bg[data_027c0280->layer1Bg] = 1;
  data_027c0280->winOut.bg[data_027c0280->layer2Bg] = 0;
  data_027c0280->winOut.bg[data_027c0280->layer3Bg] = 0;
  data_027c0280->winOut.obj = 1;
  data_027c0280->win0In.effect = 1;
  data_027c0280->win1In.effect = 1;
  data_027c0280->winOut.effect = 1;
}
