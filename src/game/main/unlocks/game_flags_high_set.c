#include "types.h"
#include "game/unlocks.h"

extern u8 gGameFlagsHigh[];

/* Sets or clears flag `id` of game progress flags 95-158 (id counted from 95). */
void GameFlagsHigh_Set(u32 id, u8 value) {
  if (value) {
    gGameFlagsHigh[id >> 3] |= 1 << (id & 7);
  } else {
    /* needed to match: -1 ^ bit, not ~bit */
    gGameFlagsHigh[id >> 3] &= -1 ^ (1 << (id & 7));
  }
}
