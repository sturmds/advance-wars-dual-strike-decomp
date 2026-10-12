#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map_edit.h"

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;
extern s16 Ov006_gDirStepX[4];
extern s16 Ov006_gDirStepY[4];

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Straight pieces and end pieces. */
#define IS_STRAIGHT_OR_END(c)                                      \
  ((c) == 0x142 || (c) == 0x143 || (c) == 0x121 || (c) == 0x120 || \
   (c) == 0x103 || (c) == 0x102)

/* Looks at the neighbour of tile x, y in direction dir (DESIGN_DIR_*) and
 * returns a PIPE_LINK_* value. The cell is read before the bounds test. */
int Ov006_GetPipeLink(int x, int y, int dir) {
  Unk_02080440_48* state;
  MapTiles* tiles;
  int nx;
  int ny = y + Ov006_gDirStepY[dir];
  int cell;
  int link;

  state = STATE;
  tiles = state->mapTiles;
  nx = x + Ov006_gDirStepX[dir];
  cell = tiles->mapCells[nx + tiles->rowStart[ny]];
  link = PIPE_LINK_NONE;

  if (nx < 0 || nx >= state->unk_1c4->mapWidth || ny < 0 ||
      ny >= state->unk_1c4->mapHeight) {
    return PIPE_LINK_NONE;
  }
  if (MAP_CELL_IS_PIPE(cell)) {
    link = PIPE_LINK_CLOSED;
    if (dir == DESIGN_DIR_LEFT) {
      if (cell == 0x142 || cell == 0x140 || cell == 0x160 || cell == 0x162 ||
          cell == 0x120 || cell == 0x121 || cell == 0x122) {
        return PIPE_LINK_OPEN;
      }
      if (IS_STRAIGHT_OR_END(cell)) {
        return PIPE_LINK_ACROSS;
      }
    } else if (dir == DESIGN_DIR_RIGHT) {
      if (cell == 0x142 || cell == 0x141 || cell == 0x161 || cell == 0x162 ||
          cell == 0x120 || cell == 0x121 || cell == 0x122) {
        return PIPE_LINK_OPEN;
      }
      if (IS_STRAIGHT_OR_END(cell)) {
        return PIPE_LINK_ACROSS;
      }
    } else if (dir == DESIGN_DIR_UP) {
      if (cell == 0x143 || cell == 0x140 || cell == 0x141 || cell == 0x163 ||
          cell == 0x123 || cell == 0x102 || cell == 0x103) {
        return PIPE_LINK_OPEN;
      }
      if (IS_STRAIGHT_OR_END(cell)) {
        return PIPE_LINK_ACROSS;
      }
    } else if (dir == DESIGN_DIR_DOWN) {
      if (cell == 0x143 || cell == 0x160 || cell == 0x161 || cell == 0x163 ||
          cell == 0x123 || cell == 0x102 || cell == 0x103) {
        return PIPE_LINK_OPEN;
      }
      if (IS_STRAIGHT_OR_END(cell)) {
        return PIPE_LINK_ACROSS;
      }
    }
  }
  return link;
}
