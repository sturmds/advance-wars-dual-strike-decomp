#include "types.h"
#include "game/Unk_ov006_0235f690.h"

void func_ov006_02350f10(void);

void func_ov006_023543c4(void) {
  Unk_ov006_0235f690* t0 = data_ov006_0236d640;
  int t1 = t0->unk_00 | 4;
  t0->unk_00 = t1;
  func_ov006_02350f10();
}
