#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

int LuckDamage(int playerIndex, int coSlot);
int Player_GetTagStars(int playerIndex);

/* Good luck roll maximum of a player: 5 per tag star, skills, and the
 * front CO's value for its current power state. 10 when coEffectsOn is clear. */
int Player_GetGoodLuck(int playerIndex) {
  int luck = 0;

  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn == 0) {
    return 10;
  }
  luck += Player_GetTagStars(playerIndex) * 5;
  luck += LuckDamage(playerIndex, 0);
  return luck + gCoTable[data_027c027c->unk_48[data_027c0284]
                             ->players[playerIndex]
                             .front.id &
                         PLAYER_CO_ID_MASK]
                    .blocks[data_027c027c->unk_48[data_027c0284]
                                ->players[playerIndex]
                                .front.powerState]
                    .goodLuck;
}
