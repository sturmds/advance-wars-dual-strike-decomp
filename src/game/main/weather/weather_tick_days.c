#include "types.h"
#include "game/Unk_02080440.h"

BOOL Player_IsInGame(u8 player);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])
#define WEATHER (data_027c027c->unk_5c[data_027c0284])

/* Counts down the current player's weather days. TRUE when they reach 0 and
 * every other player in the game is at 0 too: the weather has run its course. */
BOOL Weather_TickDays(void) {
  int player;
  int days;

  if (WEATHER->weatherDays[STATE->currentPlayer] != 0) {
    WEATHER->weatherDays[STATE->currentPlayer]--;
  }
  days = WEATHER->weatherDays[STATE->currentPlayer];
  if (days > 0) {
    return FALSE;
  }
  for (player = 1; player <= 4; player++) {
    if (player == STATE->currentPlayer || !Player_IsInGame(player)) {
      continue;
    }
    if (days != WEATHER->weatherDays[player]) {
      break;
    }
  }
  if (player <= 4) {
    return FALSE;
  }
  return TRUE;
}
