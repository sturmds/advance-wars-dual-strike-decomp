#include "types.h"
#include "game/Unk_02080440.h"

void MapCursor_SlideToward(s16 x, s16 y, int mode, s16* outX, s16* outY);
void MapCursor_DrawAt(s16 x, s16 y, int mode);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern s16 data_0229eae0[];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Name is a guess. Takes the tile in cursorX/cursorY (or data_0229eae0 when
 * mode is 3) in pixels (tile * 16), has MapCursor_SlideToward work out a position
 * from it and the unk_2c/unk_2e pair, and passes that to MapCursor_DrawAt. The
 * AiShowAction steps call it to point the view at a unit. */
void Map_ScrollViewToTile(int mode) {
  s16* tile;
  s16 outX;
  s16 outY;

  if (mode == 3) {
    tile = data_0229eae0;
  } else {
    tile = &STATE->cursorX;
  }
  MapCursor_SlideToward(tile[0] << 4, tile[1] << 4, mode, &outX, &outY);
  MapCursor_DrawAt(outX, outY, mode);
}
