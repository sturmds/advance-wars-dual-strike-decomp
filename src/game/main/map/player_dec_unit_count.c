#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

/* Takes one off a player's unit count (Player.unitCount), down to 0. */
void Player_DecUnitCount(int player) {
  Player* players = MAP_STATE->players;

  if (players[player].unitCount != 0) {
    players[player].unitCount--;
  }
}
