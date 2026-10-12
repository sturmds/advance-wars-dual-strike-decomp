#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

void Number_Draw(s16 x, s16 y, u16* map, u32 value, u16 entryBits,
                 u16 digitSet);

/* Draws value in the layer 0 tile map so that it ends at column x of row y. */
void Layer0_PutNumber(int x, int y, u32 value) {
  Number_Draw(x, y, data_027c0280->layer0Map, value, 0, 0);
}
