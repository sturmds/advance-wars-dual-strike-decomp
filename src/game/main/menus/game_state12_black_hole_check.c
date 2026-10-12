#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void ObjectHealScan_Start(void);

/* Handler for game state 12. Calls ObjectHealScan_Start when the current player is
 * Black Hole, then switches to state 10. */
void Game_State12_BlackHoleCheck(void) {
  if (STATE->players[STATE->currentPlayer].armyColour == ARMY_BLACK_HOLE) {
    ObjectHealScan_Start();
  }
  STATE->unk_18 = 10;
}
