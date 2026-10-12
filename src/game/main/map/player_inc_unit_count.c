#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Adds one to a player's unit count (Player.unitCount), up to 255. */
void Player_IncUnitCount(int player) {
  Player* players = STATE->players;

  if (players[player].unitCount < 255) {
    players[player].unitCount++;
  }
}
