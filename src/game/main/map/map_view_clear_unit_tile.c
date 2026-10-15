#include "types.h"
#include "game/Unk_02080440.h"
#include "game/Unk_02088d04.h"

/* Clears the 2 by 2 characters of map tile (x, y) on layer 1 (the unit layer).
 * The layer is 64 characters wide: columns 16-31 of the scroll window sit in
 * the second 32 by 32 block, 0x400 entries further on. */
void MapView_ClearUnitTile(u16 x, u16 y) {
  int col = x - MAP_STATE->unk_1c4->unk_26;
  int row = y - MAP_STATE->unk_1c4->unk_28;
  u16 colOff = (col & 15) * 2;
  u16 rowOff = (row & 15) << 6;
  u16 block = (col & 16) ? 0x400 : 0;
  u16* map = data_027c0280->layer1Map;
  u16* entry;

  /* Needed to match: the first store spells out the whole address. */
  *(map + block + colOff + rowOff) = 0;
  entry = map + block + colOff + rowOff;
  entry[1] = 0;
  entry[32] = 0;
  entry[33] = 0;
}
