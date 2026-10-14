#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void func_020c1c14(void) {
  Unk_02080440* t0 = data_027c027c;
  int t1 = data_027c0284;
  t0->unk_48[t1]->cycleUnitIndex = 0;
}
