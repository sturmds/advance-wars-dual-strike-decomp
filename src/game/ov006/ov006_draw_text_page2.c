#include "types.h"
#include "game/Unk_02088d04.h"

void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);
void Text_PrintImmediate(int x, int y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void Gfx_RequestLayerCopy(int layer);

/* Second page of Ov006_UpdateTextPages: clears layer 0 and prints five lines
 * of text from column 7. */
void Ov006_DrawTextPage2(void) {
  Bg_FillRect(data_027c0280->layer0Map, 0, 0, 32, 24, 0);
  Text_PrintImmediate(7, 5, data_027c0280->layer0Map, 0x7e00000c, 0, 0);
  Text_PrintImmediate(7, 7, data_027c0280->layer0Map, 0x7e00000d, 0, 0);
  Text_PrintImmediate(7, 9, data_027c0280->layer0Map, 0x7e000010, 0, 0);
  Text_PrintImmediate(7, 11, data_027c0280->layer0Map, 0x7e00000e, 0, 0);
  Text_PrintImmediate(7, 13, data_027c0280->layer0Map, 0x7e00000f, 0, 0);
  Gfx_RequestLayerCopy(0);
}
