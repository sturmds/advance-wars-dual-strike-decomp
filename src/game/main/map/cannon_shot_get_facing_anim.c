#include "types.h"

/* First shot animation for a cannon facing (MapObject.variant 0-3): 0, 1,
 * 4 or 5. The muzzle flash plays it, the impact the one 2 after it. */
int CannonShot_GetFacingAnim(u32 facing) {
  switch (facing) {
    case 0:
    default:
      return 0;
    case 1:
      return 1;
    case 2:
      return 4;
    case 3:
      return 5;
  }
}
