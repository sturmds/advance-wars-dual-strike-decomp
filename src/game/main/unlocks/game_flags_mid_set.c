#include "types.h"
#include "game/unlocks.h"
#include "game/save_progress.h"

/* Sets flag `id` of game progress flags 32-94 (id counted from 32) to value (0 or 1). */
void GameFlagsMid_Set(u32 id, u8 value) {
  int bit = 1 << (id & 7);
  gGameFlagsMid.value[id >> 3] =
      (gGameFlagsMid.value[id >> 3] & ~bit) | (bit * value);
}
