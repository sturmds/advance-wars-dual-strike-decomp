#include "types.h"
#include "game/unlocks.h"

u8 CoColor_IsSet(
    u32 coId); /* tests CO coId's bit in gCoColorFlags (see CoColor_Set) */

/* Battle maps shop condition for a CO colour: returns -1 if already bought,
 * else 1 if CO coId is unlocked and game flag 0x23 (probably the crystal
 * smashed in the campaign) is set, else 0. */
int Shop_SmashedCrystal(u32 coId) {
  if (CoColor_IsSet(coId)) {
    return -1;
  }
  /* needed to match: == 0, not ! */
  if (Unlock_IsSet(coId) == 0) {
    return 0;
  }
  return GameFlag_IsSet(0x23) != 0;
}
