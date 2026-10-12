#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;
void Number_Draw(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                 u16 digitSet);
void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);

void func_02026ddc(int x, int y, u32 value) {
  Number_Draw(x - 3, y, data_027c0280->layer0Map, value, 0, 0);
  Text_PrintImmediate(x - 2, y, data_027c0280->layer0Map, 0x79000001, 0, 0);
}
