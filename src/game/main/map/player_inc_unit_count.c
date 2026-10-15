#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

/* Adds one to a player's unit count (Player.unitCount), up to 255. */
void Player_IncUnitCount(int player) {
  Player* players = MAP_STATE->players;

  if (players[player].unitCount < 255) {
    players[player].unitCount++;
  }
}
