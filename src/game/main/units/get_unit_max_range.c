#include "types.h"
#include "game/Unk_02080440.h"

int GetCoUnitMaxRange(int player, int coSlot, int powerState, int unitType);

/* Maximum firing range of a unit type for a player, from its front CO in
 * that CO's current power state. */
int GetUnitMaxRange(int player, int unitType) {
  return GetCoUnitMaxRange(
      player, 0,
      data_027c027c->unk_48[data_027c0284]->players[player].front.powerState,
      unitType);
}
