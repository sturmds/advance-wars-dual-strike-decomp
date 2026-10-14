#include "types.h"
#include "game/Unk_ov006_0235f690.h"

int func_ov006_023540ec(void) {
  Unk_ov006_0235f690* t0 = data_ov006_0236d640;
  u16 t1 = t0->unk_00;
  int t2 = t1 | 0x4000;
  t0->unk_00 = t2;
  return t2;
}
