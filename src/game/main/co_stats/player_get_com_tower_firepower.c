#include "types.h"
#include "game/co_data.h"
#include "game/Unk_02080440.h"

int TowerFirepower(int playerIndex, int coSlot);

/* Com Tower firepower in percent of one CO of a player (slot 0 front,
 * 1 back) in a power state: (the CO's value per tower, or 10 when
 * coEffectsOn is clear, plus skills) times the towers the player owns. */
int Player_GetComTowerFirepower(int playerIndex, int slot, int powerState) {
  int perTower = 0;

  if (data_027c027c->unk_48[data_027c0284]->coEffectsOn == 0) {
    perTower += 10;
  } else {
    perTower += gCoTable[(&data_027c027c->unk_48[data_027c0284]
                               ->players[playerIndex]
                               .front)[slot]
                             .id &
                         PLAYER_CO_ID_MASK]
                    .blocks[powerState]
                    .towerFirepower;
  }
  perTower += TowerFirepower(playerIndex, slot);
  return perTower *
         data_027c027c->unk_48[data_027c0284]->players[playerIndex].comTowers;
}
