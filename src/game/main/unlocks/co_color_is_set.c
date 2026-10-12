#include "types.h"
#include "game/unlocks.h"

extern u8 gCoColorFlags[]; /* one bit per CO: alternate colour unlocked */

u8 CoColor_IsSet(u32 coId) {
  return (1 << (coId & 7)) & gCoColorFlags[coId >> 3];
}
