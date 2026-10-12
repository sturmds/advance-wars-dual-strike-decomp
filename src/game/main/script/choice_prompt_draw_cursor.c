#include "types.h"
#include "game/Unk_0201a774.h"

/* Clears the cursor tiles at columns 0 and 4, draws the cursor (tiles
 * 0x1b1 over 0x1b2) at column 4 * cursor, then calls redraw. */
void ChoicePrompt_DrawCursor(Unk_0201a774* prompt) {
  u16(*tiles)[32] = prompt->tiles;

  tiles[0][0] = 0;
  tiles[1][0] = 0;
  tiles[0][4] = 0;
  tiles[1][4] = 0;
  tiles[0][prompt->cursor * 4] = prompt->palette | 0x1b1;
  tiles[1][prompt->cursor * 4] = prompt->palette | 0x1b2;
  prompt->redraw();
}
