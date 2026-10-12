#include "types.h"
#include "game/unlocks.h"

extern u8 gCoColorFlags[]; /* one bit per CO: alternate colour unlocked */

void CoColor_Set(u32 coId, int value) {
  int bit = 1 << (coId & 7);
  gCoColorFlags[coId >> 3] = (~bit & gCoColorFlags[coId >> 3]) | (bit * value);
}
