#include "types.h"
#include "game/unlocks.h"

u8 CoColor_IsSet(
    u32 coId); /* tests CO coId's bit in gCoColorFlags (see CoColor_Set) */

/* Battle maps shop condition for a CO colour: returns -1 if already bought,
 * else 1 if CO coId is unlocked, else 0. */
int Shop_UnlockedCo(u32 coId) {
  if (CoColor_IsSet(coId)) {
    return -1;
  }
  return Unlock_IsSet(coId) != 0;
}
