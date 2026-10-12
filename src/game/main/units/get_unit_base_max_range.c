#include "types.h"

int GetCoUnitMaxRange(int player, int coSlot, int powerState, int unitType);

/* Like GetUnitMaxRange, but with the front CO's power state taken as 0
 * (no power on). The AI uses it for threat and reach estimates. */
int GetUnitBaseMaxRange(int player, int unitType) {
  return GetCoUnitMaxRange(player, 0, 0, unitType);
}
