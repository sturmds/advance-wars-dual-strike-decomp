#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen.h"

void NumberFont_LoadLow(void);
void UnitIcons_LoadMapGraphics(void);
void func_020f88b8(void);
void func_020f88ec(void);
void func_020f8848(void);
void func_020f882c(void);

extern u8 data_02166dc8[];

void func_020f869c(int mode) {
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
  NumberFont_LoadLow();
  UnitIcons_LoadMapGraphics();
  func_020f88b8();
  func_020f88ec();
  func_020f8848();
  func_020f882c();
  data_027c0280->masterBrightness = 0;
}
