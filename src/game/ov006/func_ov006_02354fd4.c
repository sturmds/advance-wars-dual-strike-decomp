#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void Ov006_PressMenuButton(int, int, void*);

void func_ov006_02354fd4(u8* a0) {
  Unk_ov006_0235f690* t0 = data_ov006_0236d640;
  int t1 = t0->unk_00 | 2;
  t0->unk_00 = t1;
  Ov006_PressMenuButton(*a0, t1, t0);
}
