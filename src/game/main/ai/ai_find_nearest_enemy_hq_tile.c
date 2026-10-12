#include "types.h"
#include "game/Unk_02080440.h"
#include "game/player.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

BOOL Player_IsInGame(u8 player);
BOOL Player_HasHq(int player);

#define STATE (data_027c027c->unk_48[data_027c0284])

#define NO_TILE 0x7fff
#define HQ_COORD_MASK 0x7f

/* Finds the HQ of an opposing player in game, among those with a
 * non-negative value in the range map, that has the lowest range map
 * value (the last one on a tie), and writes its x and y to out. FALSE when
 * there is none. */
BOOL Ai_FindNearestEnemyHqTile(s16* out) {
  int best;
  int player;
  int x;
  int y;
  s8 value;

  best = NO_TILE;
  for (player = 1; player < 5; player++) {
    if (!Player_IsInGame(player)) {
      continue;
    }
    if (STATE->players[STATE->currentPlayer].team ==
        STATE->players[player].team) {
      continue;
    }
    if (!Player_HasHq(player)) {
      continue;
    }
    x = STATE->players[player].hqX & HQ_COORD_MASK;
    y = STATE->players[player].hqY & HQ_COORD_MASK;
    value = STATE->rangeMap[y][x];
    if (value < 0) {
      continue;
    }
    if (value <= best) {
      out[0] = x;
      best = value;
      out[1] = y;
    }
  }
  return best != NO_TILE;
}
