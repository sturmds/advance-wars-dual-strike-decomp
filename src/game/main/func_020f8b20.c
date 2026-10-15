#include "types.h"
#include "game/Unk_02088d04.h"

int func_020f8b20(void) {
  Unk_027c0280* t0 = data_027c0280;
  u16 t1 = t0->dispCnt;
  int t2 = t1 | 0x200;
  t0->dispCnt = t2;
  return t2;
}
