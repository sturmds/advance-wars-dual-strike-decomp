#include "types.h"

extern u16* data_ov006_0236d640;

int func_ov006_023540ec(void) {
  u16* t0 = data_ov006_0236d640;
  u16 t1 = *t0;
  int t2 = t1 | 0x4000;
  *t0 = t2;
  return t2;
}
