#include "types.h"
#include "game/Unk_02080440.h"

void ObjectHealScan_Start(void);

/* Handler for game state 12. Calls ObjectHealScan_Start when the current player is
 * Black Hole, then switches to state 10. */
void Game_State12_BlackHoleCheck(void) {
  if (MAP_STATE->players[MAP_STATE->currentPlayer].armyColour ==
      ARMY_BLACK_HOLE) {
    ObjectHealScan_Start();
  }
  MAP_STATE->unk_18 = 10;
}
