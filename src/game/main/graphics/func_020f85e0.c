#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen.h"

void UnitIcons_LoadMapGraphics(void);
void func_020f88ec(void);
void MapBg_LoadGraphics(void);

extern u8 data_02166dc8[];

void func_020f85e0(int mode) {
  data_027c0280->masterBrightness = -16;
  if (mode == 1) {
    Screen_FillBlack();
  }
  if (mode == 0) {
    Screen_FillWhite();
  }
  Screens_SetUp2dLayers(data_02166dc8);
  data_027c0280->dispCnt |= 0x40;
  data_027c0280->unk_dd = 0;
  Screen_ResetLayerScroll();
  if (data_027c027c->unk_50[data_027c0284]->callback != NULL) {
    data_027c027c->unk_50[data_027c0284]->callback();
  }
  Screen_ApplyAll();
  UnitIcons_LoadMapGraphics();
  func_020f88ec();
  MapBg_LoadGraphics();
  data_027c0280->masterBrightness = 0;
}
