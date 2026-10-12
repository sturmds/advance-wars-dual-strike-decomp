#include "types.h"

extern u16* data_ov006_0236d640;
void func_ov006_02350f10(void);

void func_ov006_023543c4(void) {
  u16* t0 = data_ov006_0236d640;
  int t1 = *t0 | 4;
  *t0 = t1;
  func_ov006_02350f10();
}
