#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

/* Records the tile after the digit font and flags the font state as changed. */
void Font_SetDigitEndTile(int tile) {
  FontState* font = data_027c0280->fontState;

  font->digitEndTile = tile;
  font->unk_00 = 1;
}
