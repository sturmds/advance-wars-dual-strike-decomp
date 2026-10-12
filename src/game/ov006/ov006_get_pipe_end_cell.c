#include "types.h"
#include "game/Unk_02080440.h"
#include "game/terrain.h"
#include "game/design_map_edit.h"

int Ov006_IsJoinedPieceConnected(int x, int y);
void Ov006_RemoveProperty(int x, int y);
void Ov006_SetTerrain(int x, int y, int terrain);
int Ov006_FitPipeCell(int x, int y, int arg);
void Ov006_StoreMapCell(int x, int y, int cell);
int Ov006_GetPipeLink(int x, int y, int dir);

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define LINK(dir) Ov006_GetPipeLink(x, y, DESIGN_DIR_##dir)
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])

/* For the pipe at x, y: if it is open toward exactly one neighbour, returns
 * the end piece that opens that way; otherwise -1. A seam is first turned
 * into plain pipe, unless Ov006_IsJoinedPieceConnected refuses (then -1). */
int Ov006_GetPipeEndCell(int x, int y) {
  u16 cell = CELL_AT(x, y);

  if (!MAP_CELL_IS_PIPE(cell)) {
    return -1;
  }
  if (cell == MAP_CELL_PIPE_SEAM_H || cell == MAP_CELL_PIPE_SEAM_V) {
    if (Ov006_IsJoinedPieceConnected(x, y) == 0) {
      Ov006_RemoveProperty(x, y);
      Ov006_SetTerrain(x, y, TERRAIN_PIPE);
      Ov006_StoreMapCell(x, y, Ov006_FitPipeCell(x, y, 1));
    } else {
      return -1;
    }
  }
  if (LINK(LEFT) == PIPE_LINK_OPEN) {
    if (LINK(RIGHT) != PIPE_LINK_OPEN && LINK(UP) != PIPE_LINK_OPEN &&
        LINK(DOWN) != PIPE_LINK_OPEN) {
      return MAP_CELL_PIPE_END_LEFT;
    }
  } else if (LINK(RIGHT) == PIPE_LINK_OPEN) {
    if (LINK(LEFT) != PIPE_LINK_OPEN && LINK(UP) != PIPE_LINK_OPEN &&
        LINK(DOWN) != PIPE_LINK_OPEN) {
      return MAP_CELL_PIPE_END_RIGHT;
    }
  } else if (LINK(UP) == PIPE_LINK_OPEN) {
    if (LINK(LEFT) != PIPE_LINK_OPEN && LINK(RIGHT) != PIPE_LINK_OPEN &&
        LINK(DOWN) != PIPE_LINK_OPEN) {
      return MAP_CELL_PIPE_END_UP;
    }
  } else if (LINK(DOWN) == PIPE_LINK_OPEN) {
    if (LINK(LEFT) != PIPE_LINK_OPEN && LINK(RIGHT) != PIPE_LINK_OPEN &&
        LINK(UP) != PIPE_LINK_OPEN) {
      return MAP_CELL_PIPE_END_DOWN;
    }
  }
  return -1;
}
