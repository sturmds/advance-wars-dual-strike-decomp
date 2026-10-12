#include "types.h"
#include "game/unlocks.h"

void Unlock_Set(u32 id, int value) {
  int bit = 1 << (id & 7);
  gUnlockFlags[id >> 3] = (~bit & gUnlockFlags[id >> 3]) | (bit * value);
}
