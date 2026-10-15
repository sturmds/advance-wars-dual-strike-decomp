#include "types.h"
#include "game/Unk_02080440.h"

/* Cell codes of the two pipe seam pieces the Design Room tells apart (the
 * 0x142 and 0x143 pipe cells turn into them), and the seam cell used for
 * every other cell. */
#define CELL_PIPE_PIECE_A 0x142
#define CELL_PIPE_PIECE_B 0x143
#define CELL_SEAM_A 0x162
#define CELL_SEAM_B 0x163
#define CELL_SEAM_DEFAULT 0x180

/* The cell code a pipe seam gets on tile x, y, from the cell there now. */
int Ov006_GetPipeSeamCell(int x, int y) {
  u16 cell =
      MAP_STATE->mapTiles->mapCells[x + MAP_STATE->mapTiles->rowStart[y]];

  if (cell == CELL_PIPE_PIECE_A || cell == CELL_SEAM_A) {
    return CELL_SEAM_A;
  }
  if (cell == CELL_PIPE_PIECE_B || cell == CELL_SEAM_B) {
    return CELL_SEAM_B;
  }
  return CELL_SEAM_DEFAULT;
}
