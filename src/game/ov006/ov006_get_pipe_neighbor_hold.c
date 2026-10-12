#include "types.h"
#include "game/Unk_02080440.h"
#include "game/design_map_edit.h"

int Ov006_GetPipeLink(int x, int y, int dir);

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;
/* Steps per direction (DESIGN_DIR_*). Copies of Ov006_gDirStepX and
 * Ov006_gDirStepY that only this function reads. */
extern s16 data_ov006_0236d608[4];
extern s16 data_ov006_0236d5f8[4];

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])

#define FIRM 2 /* the most this returns */

/* How firmly the neighbour of pipe tile x, y in direction dir is held by
 * other pipes, 0 to FIRM. Off the map counts as FIRM. A seam running across
 * dir is FIRM when one of its two sides is open toward it, else 0; a seam
 * running along dir is 0. A neighbour that x, y does not link to (link not
 * open or across) is FIRM. Otherwise, the number of the neighbour's other
 * sides with an open link. */
int Ov006_GetPipeNeighborHold(int x, int y, int dir) {
  int link = Ov006_GetPipeLink(x, y, dir);
  int count = 0;
  int nx = x + data_ov006_0236d608[dir];
  int ny;
  u16 cell;
  int i;

  if (nx < 0 || nx >= STATE->unk_1c4->mapWidth ||
      (ny = y + data_ov006_0236d5f8[dir]) < 0 ||
      ny >= STATE->unk_1c4->mapHeight) {
    return FIRM;
  }
  cell = CELL_AT(nx, ny);
  if (cell == MAP_CELL_PIPE_SEAM_H || cell == MAP_CELL_PIPE_SEAM_V) {
    if ((u32)dir <= DESIGN_DIR_RIGHT) {
      if (cell == MAP_CELL_PIPE_SEAM_V) {
        if (Ov006_GetPipeLink(nx, ny, DESIGN_DIR_UP) == PIPE_LINK_OPEN ||
            Ov006_GetPipeLink(x + data_ov006_0236d608[dir],
                              y + data_ov006_0236d5f8[dir],
                              DESIGN_DIR_DOWN) == PIPE_LINK_OPEN) {
          return FIRM;
        }
        return count;
      }
      return count;
    }
    if ((u32)(dir - DESIGN_DIR_UP) <= DESIGN_DIR_DOWN - DESIGN_DIR_UP) {
      if (cell == MAP_CELL_PIPE_SEAM_H) {
        if (Ov006_GetPipeLink(nx, ny, DESIGN_DIR_LEFT) == PIPE_LINK_OPEN ||
            Ov006_GetPipeLink(x + data_ov006_0236d608[dir],
                              y + data_ov006_0236d5f8[dir],
                              DESIGN_DIR_RIGHT) == PIPE_LINK_OPEN) {
          return FIRM;
        }
        return count;
      }
      return count;
    }
  }
  if (link != PIPE_LINK_ACROSS && link != PIPE_LINK_OPEN) {
    return FIRM;
  }
  for (i = 0; i < 4; i++) {
    /* Skips the side that points back at x, y. The original tests the x
         * steps twice, so for up and down it skips both up and down. */
    if (data_ov006_0236d608[dir] + data_ov006_0236d608[i] != 0 ||
        data_ov006_0236d608[dir] + data_ov006_0236d608[i] != 0) {
      if (Ov006_GetPipeLink(x + data_ov006_0236d608[dir],
                            y + data_ov006_0236d5f8[dir],
                            i) == PIPE_LINK_OPEN) {
        count++;
      }
    }
  }
  return count;
}
