#include "types.h"
#include "game/Unk_02088d04.h"

void Text_PrintImmediate(s16 x, s16 y, u16* map, u32 textId, u16 a, u16 b);

extern Unk_027c0280* data_027c0280;

/* Prints the four column headings of the unit list on layer 0 (tile row 6). */
void UnitList_DrawHeadings(void) {
  Text_PrintImmediate(9, 6, data_027c0280->layer0Map, 0xc00000b5, 0, 0);
  Text_PrintImmediate(17, 6, data_027c0280->layer0Map, 0xc00000b6, 0, 0);
  Text_PrintImmediate(20, 6, data_027c0280->layer0Map, 0xc00000b7, 0, 0);
  Text_PrintImmediate(26, 6, data_027c0280->layer0Map, 0xc00000b8, 0, 0);
}
