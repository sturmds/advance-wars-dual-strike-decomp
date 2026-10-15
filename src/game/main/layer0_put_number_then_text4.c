#include "types.h"
#include "game/Unk_02088d04.h"

void Number_Draw(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                 u16 digitSet);
void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);

/* On row y of the layer 0 tile map: draws value so that it ends at column
 * x - 1, then prints text 0x79000004 from column x. */
void Layer0_PutNumberThenText4(int x, int y, u32 value) {
  Number_Draw(x - 1, y, data_027c0280->layer0Map, value, 0, 0);
  Text_PrintImmediate(x, y, data_027c0280->layer0Map, 0x79000004, 0, 0);
}
