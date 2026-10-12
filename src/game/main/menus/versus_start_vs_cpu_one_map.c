#include "types.h"
#include "game/screen_other_map.h"

void Versus_StartVsCpu(u8 playerCount);

/* Versus against three AI players on one map. */
void Versus_StartVsCpuOneMap(void) {
  data_027c027c->unk_48[data_027c0284]->twoMaps = 0;
  Versus_StartVsCpu(4);
}
