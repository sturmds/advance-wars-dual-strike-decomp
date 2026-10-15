#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

void Player_DecUnitCount(int player);

/* Counts a lost unit of unitType for a player and takes it off the
 * player's unit count. Unit_Remove calls it. The count stops at 255. */
void Player_CountUnitLost(int player, int unitType) {
  Player* players;

  Player_DecUnitCount(player);
  players = MAP_STATE->players;
  if (players[player].unitsLost[unitType] != 255) {
    players[player].unitsLost[unitType]++;
  }
}
