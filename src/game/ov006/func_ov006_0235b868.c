#include "types.h"
#include "game/Unk_02080440.h"

/* TRUE when the cell at x, y is 450 or 453 to 462. */
BOOL func_ov006_0235b868(int x, int y) {
  switch (MAP_STATE->mapTiles->mapCells[x + MAP_STATE->mapTiles->rowStart[y]]) {
    case 450:
      return TRUE;
    case 455:
      return TRUE;
    case 460:
      return TRUE;
    case 454:
      return TRUE;
    case 459:
      return TRUE;
    case 456:
      return TRUE;
    case 461:
      return TRUE;
    case 457:
      return TRUE;
    case 462:
      return TRUE;
    case 453:
      return TRUE;
    case 458:
      return TRUE;
  }
  return FALSE;
}
