#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void Number_Draw(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                 u16 digitSet);
void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);

/* On row y of the layer 0 tile map: draws value so that it ends at column
 * x - 4, then prints text 0x79000003 from column x - 3. */
void Layer0_PutNumberThenText3(int x, int y, u32 value) {
  Number_Draw(x - 4, y, data_027c0280->layer0Map, value, 0, 0);
  Text_PrintImmediate(x - 3, y, data_027c0280->layer0Map, 0x79000003, 0, 0);
}
