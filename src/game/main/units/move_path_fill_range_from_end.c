#include "types.h"
#include "game/Unk_02080440.h"
#include "game/move_path.h"

extern MovePath* gMovePath;
extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define GAME (data_027c027c->unk_48[data_027c0284])

void RangeMap_FillMovesForPlayer(int player, int x, int y, int unitType,
                                 int movesLeft, int flag);

/* Calls RangeMap_FillMovesForPlayer (guess: fills the movement range) for the acting
 * unit from the last tile of the move path, with the move points left
 * there. */
void MovePath_FillRangeFromEnd(void) {
  RangeMap_FillMovesForPlayer(
      (u16)((GAME->actingUnitIndex >> 6) + 1),
      gMovePath->x[gMovePath->lastTile], gMovePath->y[gMovePath->lastTile],
      GAME->actingUnit->type, gMovePath->movesLeft[gMovePath->lastTile], 1);
}
