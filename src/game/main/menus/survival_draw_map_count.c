#include "types.h"
#include "game/Unk_02088d04.h"

void Number_Draw(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                 u16 digitSet);
void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 unk_30,
                         int arg);

/* Prints a map count on layer 0 with the label text 0x79000001 right after
 * it. Counts of 10 or more are drawn and labelled one column further right. */
void Survival_DrawMapCount(int x, int y, int count) {
  if (count >= 10) {
    x++;
  }
  Number_Draw(x, y, data_027c0280->layer0Map, count, 0, 0);
  Text_PrintImmediate(x + 1, y, data_027c0280->layer0Map, 0x79000001, 0, 0);
}
