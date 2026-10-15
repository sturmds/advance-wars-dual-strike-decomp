#include "types.h"
#include "game/Unk_02088d04.h"

void Bg_PutTileBlock(u16* map, int x, int y, const u8* block, u16 tileAdd);

/* Draws the frame of one player column (1-4) on BG layer 1 at tile (x, y):
 * the tile block from the screen's resource file, then the 2x2 tiles at
 * (x + 3, y) moved on by 4 tiles per column (likely the player number), and
 * every tile of the block switched to palette column - 1 more. */
void CoSelectScreen_DrawColumnFrame(u8* block, u8 x, u8 y, u8 column) {
  int pos;
  int playerIndex;
  int row;
  int rowOffset;
  int col;

  Bg_PutTileBlock(data_027c0280->layer1Map, x, y, block, 0x2200);
  playerIndex = column - 1;
  pos = x + y * 32;
  data_027c0280->layer1Map[pos + 3] += playerIndex * 4;
  data_027c0280->layer1Map[pos + 4] += playerIndex * 4;
  data_027c0280->layer1Map[pos + 35] += playerIndex * 4;
  data_027c0280->layer1Map[pos + 36] += playerIndex * 4;
  rowOffset = y * 32;
  for (row = y; row <= y + block[1]; row++) {
    for (col = x; col <= x + block[0]; col++) {
      data_027c0280->layer1Map[col + rowOffset] += playerIndex << 12;
    }
    rowOffset += 32;
  }
}
