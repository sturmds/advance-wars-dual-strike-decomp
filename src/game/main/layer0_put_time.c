#include "types.h"
#include "game/Unk_02088d04.h"

extern u8 gColonText[];

void Number_Draw(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                 u16 digitSet);
void Number_DrawPadded(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                       u16 digitSet, int digits);
void Text_PrintStringImmediate(s16 x, s16 y, u16* map, const void* text,
                               u16 unk_30, int arg);

/* On row y of the layer 0 tile map, ending at column x: hours, separator,
 * two padded digits of minutes, separator, two padded digits of seconds. */
void Layer0_PutTime(int x, int y, u32 hours, u32 minutes, u32 seconds) {
  Number_Draw(x - 6, y, data_027c0280->layer0Map, hours, 0, 0);
  Text_PrintStringImmediate(x - 5, y, data_027c0280->layer0Map, gColonText, 0,
                            0);
  Number_DrawPadded(x - 3, y, data_027c0280->layer0Map, minutes, 0, 0, 2);
  Text_PrintStringImmediate(x - 2, y, data_027c0280->layer0Map, gColonText, 0,
                            0);
  Number_DrawPadded(x, y, data_027c0280->layer0Map, seconds, 0, 0, 2);
}
