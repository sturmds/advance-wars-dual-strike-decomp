#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"
#include "game/screen_other_map.h"

void Gfx_RequestLayerCopy(int layer);
void Bg_ClearRect(void* target, int x, int y, int w, int h);
void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);

/* On the main screen: clears tile rows 4 to 15 of layer 0 and prints the
 * page label (text 0x7a0001c7, one past the first page's) at tile 4, 4. */
void ResultStats_PrintSecondPageLabel(void) {
  Unk_027c0280* saved0280 = data_027c0280;
  Unk_02080440* saved027c;

  Screen_SetCurrentGfx(data_027c0288.unk_00);
  saved027c = data_027c027c;
  Screen_SetCurrent(data_027c0288.unk_00);
  Bg_ClearRect(data_027c0280->layer0Map, 1, 4, 31, 12);
  Text_PrintImmediate(4, 4, data_027c0280->layer0Map, 0x7a0001c7, 0, 0);
  Gfx_RequestLayerCopy(0);
  Screen_SetCurrent(saved027c);
  Screen_SetCurrentGfx(saved0280);
}
