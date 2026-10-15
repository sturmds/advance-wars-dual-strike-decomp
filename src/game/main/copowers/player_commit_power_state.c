#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

/* Copies the front CO's unk_12 (the power state that func_020e1acc stores
 * before it runs the power) into powerState. */
void Player_CommitPowerState(int playerIndex) {
  Player* player = &data_027c027c->unk_48[data_027c0284]->players[playerIndex];

  player->front.powerState = player->front.unk_12;
}
