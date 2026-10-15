#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

/* Com Tower defense in percent of one CO of a player (slot 0 front, 1 back)
 * in a power state: the CO's value per tower times the towers owned.
 * 0 with coEffectsOn clear. */
int Player_GetComTowerDefense(int playerIndex, int slot, int powerState) {
  int perTower = 0;

  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn != 0) {
    perTower += gCoTable[(&data_027c027c->unk_48[data_027c0284]
                               ->players[playerIndex]
                               .front)[slot]
                             .id &
                         PLAYER_CO_ID_MASK]
                    .blocks[powerState]
                    .towerDefense;
  }
  return perTower *
         data_027c027c->unk_48[data_027c0284]->players[playerIndex].comTowers;
}
