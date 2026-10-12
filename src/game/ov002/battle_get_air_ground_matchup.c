#include "types.h"

BOOL Battle_IsSideAir(int side);

/* How the side's unit and the other side's unit differ in altitude: 0 both
 * air or both ground, 1 this side on the ground and the other in the air,
 * 2 this side in the air and the other on the ground. */
int Battle_GetAirGroundMatchup(int side) {
  if (Battle_IsSideAir(side) == 1) {
    return Battle_IsSideAir((side + 1) & 1) == 1 ? 0 : 2;
  }
  return Battle_IsSideAir((side + 1) & 1) == 1 ? 1 : 0;
}
