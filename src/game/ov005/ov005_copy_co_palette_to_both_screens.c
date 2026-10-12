#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bba8.h"

void Co_CopyPalette(int coId, int paletteSlot, int palette);

void Ov005_CopyCoPaletteToBothScreens(Unk_ov005_0236bba8* work) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Co_CopyPalette(work->coId, 28, work->unk_28);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Co_CopyPalette(work->coId, 28, work->unk_28);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
