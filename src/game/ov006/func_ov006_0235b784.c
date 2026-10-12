#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* TRUE when x, y is off the map or its cell is 0, 19, 42 or 360. */
BOOL func_ov006_0235b784(int x, int y) {
  if (x < 0) {
    return TRUE;
  }
  if (x >= STATE->unk_1c4->mapWidth) {
    return TRUE;
  }
  if (y < 0) {
    return TRUE;
  }
  if (y >= STATE->unk_1c4->mapHeight) {
    return TRUE;
  }
  switch (STATE->mapTiles->mapCells[x + STATE->mapTiles->rowStart[y]]) {
    case 42:
      return TRUE;
    case 0:
      return TRUE;
    case 360:
      return TRUE;
    case 19:
      return TRUE;
  }
  return FALSE;
}
