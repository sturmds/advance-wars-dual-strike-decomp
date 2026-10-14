#include "types.h"
#include "game/Unk_ov005_0236bbac.h"

extern Unk_ov005_0236bbac* data_ov005_0236bbac;

int func_ov005_023659b8(u8* a0) {
  u8 t0 = *a0;
  Unk_ov005_0236bbac* t1 = data_ov005_0236bbac;
  int t2 = t0 - 128;
  t1->selection = t2;
  t1->aTouched = 1;
  return t2;
}
