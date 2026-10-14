#include "types.h"
#include "game/Unk_02088d04.h"

extern Unk_027c0280* data_027c0280;

int func_0200c0ec(void) {
  Unk_027c0280* t0 = data_027c0280;
  u16 t1 = t0->dispStat;
  int t2 = t1 | 16;
  t0->dispStat = t2;
  return t2;
}
