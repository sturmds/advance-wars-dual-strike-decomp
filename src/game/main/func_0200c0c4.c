#include "types.h"
#include "game/Unk_02088d04.h"

void OS_DisableIrqMask(int, int, void*);

void func_0200c0c4(void) {
  Unk_027c0280* t0 = data_027c0280;
  int t1 = t0->dispStat & -17;
  t0->dispStat = t1;
  OS_DisableIrqMask(2, t1, t0);
}
