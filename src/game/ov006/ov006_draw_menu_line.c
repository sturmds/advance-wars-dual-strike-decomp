#include "types.h"
#include "game/Unk_02088d04.h"

void Bg_FillRect(u16* map, int x, int y, int width, int height, int tile);
void Text_PrintStringImmediate(s16 x, s16 y, u16* map, const char* text, u16 a,
                               u16 b);
void Gfx_RequestLayerCopy(int layer);

/* Clears a 9x2 box on layer 0 and prints text in it. Menu row `row` starts at
 * tile row 6 + 2 * row; the box starts at column 4 + indent. */
void Ov006_DrawMenuLine(int unused, int row, const char* text, int indent) {
  int y = 6;

  y += row * 2;
  Bg_FillRect(data_027c0280->layer0Map, indent + 4, y, 9, 2, 0);
  Text_PrintStringImmediate(indent + 4, y, data_027c0280->layer0Map, text, 0,
                            0);
  Gfx_RequestLayerCopy(0);
}
