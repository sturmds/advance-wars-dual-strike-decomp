#include "types.h"
#include "game/Unk_02088d04.h"
#include "game/text_print.h"
#include "game/screen_other_map.h"

void Number_Draw(s16 x, s16 y, u16* map, int value, u16 a, u16 b);
void Gfx_RequestLayerCopy(int layer);
void TabList_CountMedals(u16* earned, u16* total);

extern char data_02167c88[];

/* Prints a heading and "earned / total" medals on layer 0 of the other screen. */
void TabList_DrawMedalCount(void) {
  u16 earned;
  u16 total;
  Unk_027c0280* saved;

  TabList_CountMedals(&earned, &total);
  saved = data_027c0280;
  Screen_SetCurrentGfx(data_027c0288.unk_04);
  Text_PrintId(26, 9, data_027c0280->layer0Map, 0x7a000000, 0, 0x1c0);
  Number_Draw(26, 11, data_027c0280->layer0Map, earned, 0, 0);
  Text_Print(27, 11, data_027c0280->layer0Map, data_02167c88, 0, 0x1f0);
  Number_Draw(30, 11, data_027c0280->layer0Map, total, 0, 0);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrentGfx(saved);
}
