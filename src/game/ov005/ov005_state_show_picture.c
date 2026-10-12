#include "types.h"
#include "game/screen_other_map.h"
#include "game/Unk_ov005_0236bba4.h"

void Wallpaper_LoadBothOnLayer3(int picture);

/* State run function (the `run` of many entries in data_ov005_0236a74c):
 * loads picture `picture` with Wallpaper_LoadBothOnLayer3, then shows only BG3 and OBJ
 * on the main screen and only BG3 on the sub screen. */
void Ov005State_ShowPicture(Unk_ov005_0236bba4* work, int picture) {
  Unk_027c0280* saved0280;
  Unk_02080440* saved027c;

  Wallpaper_LoadBothOnLayer3((u8)picture);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  data_027c0280->dispCnt &= ~0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt &= ~0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt |= 0x1000;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);

  saved0280 = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_04);
  data_027c0280->dispCnt &= ~0x100;
  data_027c0280->dispCnt &= ~0x200;
  data_027c0280->dispCnt &= ~0x400;
  data_027c0280->dispCnt |= 0x800;
  data_027c0280->dispCnt &= ~0x1000;
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
