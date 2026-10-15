#include "types.h"
#include "game/Unk_02080440.h"

s32 Rng_Range(s32 min, s32 max);

/* A random x (vertical 0) or y (vertical 1) in the range low to high that
 * lies on the map. */
int Ov006_RandomMapCoord(int low, int high, int vertical) {
  int value;

  while (1) {
    value = Rng_Range(low, high);
    if (value < 0) {
      continue;
    }
    if (vertical) {
      if (value >= MAP_STATE->unk_1c4->mapHeight) {
        continue;
      }
    } else {
      if (value >= MAP_STATE->unk_1c4->mapWidth) {
        continue;
      }
    }
    return value;
  }
}
