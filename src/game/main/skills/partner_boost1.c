#include "types.h"
#include "game/player.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Whether a player gets the skills its tag partner lends; AI players and
 * the others each have their own switch. */
BOOL PartnerBoost1(int player) {
  Unk_02080440_48* state = data_027c027c->unk_48[data_027c0284];

  if (state->players[player].controller == PLAYER_CONTROLLER_AI) {
    return state->partnerBoostAi != 0;
  }
  return state->partnerBoostHuman != 0;
}
