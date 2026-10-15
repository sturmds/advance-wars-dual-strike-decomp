#include "types.h"
#include "game/Unk_02088d04.h"

/* Records the tile after the digit font and flags the font state as changed. */
void Font_SetDigitEndTile(int tile) {
  FontState* font = data_027c0280->fontState;

  font->digitEndTile = tile;
  font->unk_00 = 1;
}
