#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

/* Units of unitType that a player has lost. */
int Player_GetUnitsLost(int player, int unitType) {
  return MAP_STATE->players[player].unitsLost[unitType];
}
