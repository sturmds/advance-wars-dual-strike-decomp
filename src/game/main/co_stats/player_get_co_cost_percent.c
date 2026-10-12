#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

int CostReduced(int player, int slot);

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Unit cost change in percent of one CO of a player (slot 0 front, 1 back)
 * in a power state: the CO's costPercent (0 with coEffectsOn clear) plus the
 * Force Rank price cuts. unitType is passed by the callers and not used. */
int Player_GetCoCostPercent(int playerIndex, int slot, int powerState,
                            int unitType) {
  int bonus = 0;

  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn != 0) {
    int co = (&data_027c027c->unk_48[data_027c0284]
                   ->players[playerIndex]
                   .front)[slot]
                 .id &
             PLAYER_CO_ID_MASK;

    bonus += gCoTable[co].blocks[powerState].costPercent;
  }
  return bonus + CostReduced(playerIndex, slot);
}
