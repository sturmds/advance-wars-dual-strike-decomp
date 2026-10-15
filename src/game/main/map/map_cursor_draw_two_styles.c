#include "types.h"
#include "game/Unk_02080440.h"

void MapCursor_SlideToward(s16 x, s16 y, s16 mode, s16* outX, s16* outY);
void MapCursor_DrawAt(s16 x, s16 y, int mode);

extern s16 data_0229eae0[];

/* Draws the map cursor at its screen position in style mode and again in
 * secondMode (Map_ScrollViewToTile draws only the first). Mode 3 reads the
 * tile from data_0229eae0 instead of the cursor. Callers pass 2 with 18 or
 * 19, the silo and Black Bomb target pickers. */
void MapCursor_DrawTwoStyles(int mode, int secondMode) {
  s16* tile;
  s16 outX;
  s16 outY;

  if (mode == 3) {
    tile = data_0229eae0;
  } else {
    tile = &MAP_STATE->cursorX;
  }
  MapCursor_SlideToward(tile[0] << 4, tile[1] << 4, mode, &outX, &outY);
  MapCursor_DrawAt(outX, outY, mode);
  MapCursor_DrawAt(outX, outY, secondMode);
}
