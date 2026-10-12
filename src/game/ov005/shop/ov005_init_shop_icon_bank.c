#include "types.h"
#include "game/icon.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bbb0.h"

void Ov005_InitShopIconBank(Unk_ov005_0236bbb0* work) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  work->iconCount = 0;
  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  IconBank_Init(0, data_027c0280->objVram, 320, 27);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
