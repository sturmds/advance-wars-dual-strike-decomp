#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define PLAYERS (data_027c027c->unk_48[data_027c0284]->players)

/* 1 + the properties the player owns (bases, cities, airports, ports, Com Towers). */
int Player_CountPropertiesPlusOne(int player) {
  int count = 0;

  count += PLAYERS[player].unk_10[1];
  count += PLAYERS[player].unk_10[0];
  count += PLAYERS[player].unk_10[2];
  count += PLAYERS[player].unk_10[3];
  count += PLAYERS[player].comTowers;
  return count + 1;
}
