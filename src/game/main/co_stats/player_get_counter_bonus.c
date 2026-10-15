#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

/* Counter damage bonus in percent of a player: the front CO's value for
 * its current power state. 0 when coEffectsOn is clear. */
int Player_GetCounterBonus(int playerIndex) {
  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn == 0) {
    return 0;
  }
  return gCoTable
      [data_027c027c->unk_48[data_027c0284]->players[playerIndex].front.id &
       PLAYER_CO_ID_MASK]
          .blocks[data_027c027c->unk_48[data_027c0284]
                      ->players[playerIndex]
                      .front.powerState]
          .counterBonus;
}
