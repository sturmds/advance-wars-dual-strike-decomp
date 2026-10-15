#include "types.h"
#include "game/Unk_02080440.h"

int GetCoMoveBonus(int player, int coSlot, int powerState, int unitType);

/* Movement change for a unit type from a player's front CO in that CO's
 * current power state. */
int GetUnitMoveBonus(int player, int unitType) {
  return GetCoMoveBonus(
      player, 0,
      data_027c027c->unk_48[data_027c0284]->players[player].front.powerState,
      unitType);
}
