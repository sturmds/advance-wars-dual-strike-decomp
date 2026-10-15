#include "types.h"
#include "game/Unk_02088d04.h"

int func_0201d7fc(void) {
  Unk_027c0280* t0 = data_027c0280;
  u16 t1 = t0->dispStat;
  int t2 = t1 & -17;
  t0->dispStat = t2;
  return t2;
}
