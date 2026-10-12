#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void RangeMap_FillMoves(int x, int y, int unitType, int movement, int arg4);

/* RangeMap_FillMoves with unk_1e (a player index; guess: whose units count
 * as enemies on the way) set to `player` for the call, then back to the
 * current player. `movement` is an int here because the original does not
 * narrow it before the call. */
void RangeMap_FillMovesForPlayer(int player, int x, int y, int unitType,
                                 int movement, int arg4) {
  STATE->unk_1e = player;
  RangeMap_FillMoves(x, y, unitType, movement, arg4);
  STATE->unk_1e = STATE->currentPlayer;
}
