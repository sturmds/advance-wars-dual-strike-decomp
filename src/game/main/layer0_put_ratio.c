#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;
extern u8 gColonText[];

void Number_Draw(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                 u16 digitSet);
void Number_DrawPadded(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                       u16 digitSet, int digits);
void Text_PrintStringImmediate(s16 x, s16 y, u16* map, const void* text,
                               u16 unk_30, int arg);

/* On row y of the layer 0 tile map: first ends at column x - 3, a separator
 * string follows at x - 2, and second takes the two columns up to x, padded. */
void Layer0_PutRatio(int x, int y, u32 first, u32 second) {
  Number_Draw(x - 3, y, data_027c0280->layer0Map, first, 0, 0);
  Text_PrintStringImmediate(x - 2, y, data_027c0280->layer0Map, gColonText, 0,
                            0);
  Number_DrawPadded(x, y, data_027c0280->layer0Map, second, 0, 0, 2);
}
