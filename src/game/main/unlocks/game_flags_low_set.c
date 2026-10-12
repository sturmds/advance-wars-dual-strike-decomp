#include "types.h"
#include "game/unlocks.h"

extern u8 gGameFlagsLow[];

/* Sets or clears flag `id` of game progress flags 0-31. */
void GameFlagsLow_Set(int id, int value) {
  if (value) {
    gGameFlagsLow[id >> 3] |= 1 << (id & 7);
  } else {
    /* needed to match: -1 ^ bit, not ~bit */
    gGameFlagsLow[id >> 3] &= -1 ^ (1 << (id & 7));
  }
}
