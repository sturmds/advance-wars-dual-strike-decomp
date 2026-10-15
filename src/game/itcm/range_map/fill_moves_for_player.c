#include "types.h"
#include "game/Unk_02080440.h"

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);

/* RangeMap_FillMoves with unk_1e (a player index; guess: whose units count
 * as enemies on the way) set to `player` for the call, then back to the
 * current player. `movement` is an int here because the original does not
 * narrow it before the call. */
void RangeMap_FillMovesForPlayer(int player, int x, int y, int unitType,
                                 int movement, int arg4) {
  MAP_STATE->unk_1e = player;
  RangeMap_FillMoves(x, y, unitType, movement, arg4);
  MAP_STATE->unk_1e = MAP_STATE->currentPlayer;
}
