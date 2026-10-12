#include "types.h"
#include "game/Unk_02080440.h"

int func_ov006_0235b784(int x, int y);

extern Unk_02080440* data_027c027c;
extern u32 data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define CELL_AT(x, y) \
  (STATE->mapTiles->mapCells[(x) + STATE->mapTiles->rowStart[y]])

/* Map generator test on tile (x, y). func_ov006_0235b784 returns 1 for a
 * neighbour off the map or of cell code 0, 19, 42 (sea) or 360. Unless such
 * neighbours lie both left/right and above/below, returns -1. With two of
 * them (a corner): the count of the four neighbours whose cell code is 0.
 * With three: the count of neighbours of cell code 0 whose opposite
 * neighbour passes the test. Four returns -1. */
int func_ov006_0235b46c(int x, int y) {
  int vert = 0;
  int horiz = 0;
  int count;

  if (func_ov006_0235b784(x - 1, y) == 1) {
    horiz++;
  }
  if (func_ov006_0235b784(x + 1, y) == 1) {
    horiz++;
  }
  if (func_ov006_0235b784(x, y - 1) == 1) {
    vert++;
  }
  if (func_ov006_0235b784(x, y + 1) == 1) {
    vert++;
  }
  if (horiz == 0 || vert == 0) {
    return -1;
  }
  switch (vert + horiz) {
    case 0:
    case 1:
    case 4:
      return -1;
    case 2:
      count = 0;
      if (CELL_AT(x - 1, y) == 0) {
        count++;
      }
      if (CELL_AT(x + 1, y) == 0) {
        count++;
      }
      if (CELL_AT(x, y - 1) == 0) {
        count++;
      }
      if (CELL_AT(x, y + 1) == 0) {
        count++;
      }
      return count;
    case 3:
      count = 0;
      if (CELL_AT(x - 1, y) == 0 && func_ov006_0235b784(x + 1, y) == 1) {
        count++;
      }
      if (CELL_AT(x + 1, y) == 0 && func_ov006_0235b784(x - 1, y) == 1) {
        count++;
      }
      if (CELL_AT(x, y - 1) == 0 && func_ov006_0235b784(x, y + 1) == 1) {
        count++;
      }
      if (CELL_AT(x, y + 1) == 0 && func_ov006_0235b784(x, y - 1) == 1) {
        count++;
      }
      return count;
  }
  return -1;
}
