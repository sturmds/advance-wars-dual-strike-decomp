#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Takes one off a player's unit count (Player.unitCount), down to 0. */
void Player_DecUnitCount(int player) {
  Player* players = STATE->players;

  if (players[player].unitCount != 0) {
    players[player].unitCount--;
  }
}
