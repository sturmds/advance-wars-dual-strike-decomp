#include "types.h"
#include "game/Unk_02088d04.h"

void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);
void Text_PrintImmediate(int x, int y, u16* map, u32 textId, u16 unk_30,
                         int arg);
void Gfx_RequestLayerCopy(int layer);

/* First page of Ov006_UpdateTextPages: clears layer 0 and prints its text.
 * Rows 15 and 21 hold short texts 6 columns apart, from column 4. */
void Ov006_DrawTextPage1(void) {
  Bg_FillRect(data_027c0280->layer0Map, 0, 0, 32, 24, 0);
  Text_PrintImmediate(1, 1, data_027c0280->layer0Map, 0x7e000006, 0, 0);
  Text_PrintImmediate(9, 1, data_027c0280->layer0Map, 0x7e000007, 0, 0);
  Text_PrintImmediate(9, 3, data_027c0280->layer0Map, 0x7e000008, 0, 0);
  Text_PrintImmediate(1, 5, data_027c0280->layer0Map, 0x7e000009, 0, 0);
  Text_PrintImmediate(9, 5, data_027c0280->layer0Map, 0x7e00000a, 0, 0);
  Text_PrintImmediate(9, 7, data_027c0280->layer0Map, 0x7e00000b, 0, 0);
  Text_PrintImmediate(1, 9, data_027c0280->layer0Map, 0x7e00002d, 0, 0);
  Text_PrintImmediate(4, 15, data_027c0280->layer0Map, 0x7e00002f, 0, 0);
  Text_PrintImmediate(10, 15, data_027c0280->layer0Map, 0x7e00002e, 0, 0);
  Text_PrintImmediate(16, 15, data_027c0280->layer0Map, 0x7e000030, 0, 0);
  Text_PrintImmediate(22, 15, data_027c0280->layer0Map, 0x7e000031, 0, 0);
  Text_PrintImmediate(4, 21, data_027c0280->layer0Map, 0x7e000033, 0, 0);
  Text_PrintImmediate(10, 21, data_027c0280->layer0Map, 0x7e000032, 0, 0);
  Text_PrintImmediate(16, 21, data_027c0280->layer0Map, 0x7e000034, 0, 0);
  Gfx_RequestLayerCopy(0);
}
