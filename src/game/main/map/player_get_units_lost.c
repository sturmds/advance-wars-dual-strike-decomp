#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Units of unitType that a player has lost. */
int Player_GetUnitsLost(int player, int unitType) {
  return STATE->players[player].unitsLost[unitType];
}
