#include "types.h"

extern u16* data_ov006_0236d640;
void Ov006_PressMenuButton(int, int, void*);

void func_ov006_02354fd4(u8* a0) {
  u16* t0 = data_ov006_0236d640;
  int t1 = *t0 | 2;
  *t0 = t1;
  Ov006_PressMenuButton(*a0, t1, t0);
}
