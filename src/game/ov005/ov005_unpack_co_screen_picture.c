#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bba8.h"

void Co_UnpackPicture(int coId, int firstTile);
void Palette_LoadObjRow(int row);

void Ov005_UnpackCoScreenPicture(Unk_ov005_0236bba8* work) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  Co_UnpackPicture(work->coId, 0x1c0);
  Palette_LoadObjRow(10);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
