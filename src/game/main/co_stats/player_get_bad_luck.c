#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

int BadLuckDamage(int playerIndex, int coSlot);

/* Bad luck roll maximum of a player: the front CO's value for its current
 * power state plus skills. 0 when coEffectsOn is clear. */
int Player_GetBadLuck(int playerIndex) {
  int badLuck = 0;

  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn == 0) {
    return badLuck;
  }
  badLuck += BadLuckDamage(playerIndex, 0);
  return badLuck + gCoTable[data_027c027c->unk_48[data_027c0284]
                                ->players[playerIndex]
                                .front.id &
                            PLAYER_CO_ID_MASK]
                       .blocks[data_027c027c->unk_48[data_027c0284]
                                   ->players[playerIndex]
                                   .front.powerState]
                       .badLuck;
}
