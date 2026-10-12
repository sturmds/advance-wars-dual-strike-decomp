#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"
#include "game/Unk_022a7b80.h"

#define FUNDS_MAX 999999

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern Unk_022a7b80 data_022a7b80;

/* Adds amount to a player's funds and caps them at 999999. */
void Player_AddFunds(int playerIndex, int amount) {
  Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];
  Player* players = state->players;

  /* Human players get nothing while state->gameMode is 3 and
     * data_022a7b80.mode is 1 or 4 (meaning unknown). */
  if (players[playerIndex].controller == PLAYER_CONTROLLER_HUMAN &&
      state->gameMode == 3) {
    if (data_022a7b80.mode == 1 || data_022a7b80.mode == 4) {
      return;
    }
  }
  players[playerIndex].funds += amount;
  /* needed to match: the cap reloads the player through the globals and
     * compares unsigned, so negative funds are also set to the cap */
  if ((u32)data_027c027c->unk_48[data_027c0284]->players[playerIndex].funds >
      FUNDS_MAX) {
    data_027c027c->unk_48[data_027c0284]->players[playerIndex].funds =
        FUNDS_MAX;
  }
}
