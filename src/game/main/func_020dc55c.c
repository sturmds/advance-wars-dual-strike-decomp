#include "types.h"
#include "game/Unk_02080440.h"

void MapScript_RunList0c(MapUnit*, void*);

void func_020dc55c(void) {
  Unk_02080440* t0 = data_027c027c;
  int t1 = data_027c0284;
  MapScript_RunList0c(t0->unk_48[t1]->actingUnit, t0);
}
