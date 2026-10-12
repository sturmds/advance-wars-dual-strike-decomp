#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);
BOOL Map_IsSiloOrCapturableTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Fills rangeMap with the moves of the unit's type from its tile, with a
 * movement of 110 (far more than any unit has), then returns TRUE if any
 * tile it reaches is a silo or property Map_IsSiloOrCapturableTile accepts. */
BOOL AiUnit_CanReachCaptureSite(MapUnit* unit) {
  int x;
  int y;

  RangeMap_FillMoves(unit->x, unit->y, unit->type, 110, 0);
  for (y = 0; y < STATE->unk_1c4->mapHeight; y++) {
    for (x = 0; x < STATE->unk_1c4->mapWidth; x++) {
      if (STATE->rangeMap[y][x] >= 0 &&
          Map_IsSiloOrCapturableTile(x, y) == TRUE) {
        return TRUE;
      }
    }
  }
  return FALSE;
}
