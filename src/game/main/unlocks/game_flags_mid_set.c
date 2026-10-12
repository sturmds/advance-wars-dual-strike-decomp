#include "types.h"
#include "game/unlocks.h"

extern u8 gGameFlagsMid[];

/* Sets flag `id` of game progress flags 32-94 (id counted from 32) to value (0 or 1). */
void GameFlagsMid_Set(u32 id, u8 value) {
  int bit = 1 << (id & 7);
  gGameFlagsMid[id >> 3] = (gGameFlagsMid[id >> 3] & ~bit) | (bit * value);
}
