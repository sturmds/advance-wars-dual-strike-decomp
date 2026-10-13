#include "types.h"
#include "game/Unk_0216e2b0.h"

/* Byte 1 of data_0216e2b0 as a whole: unk_01 and unk_01_hi. */
#define BYTE_01 (((u8*)&data_0216e2b0)[1])

int func_02007710(int a0) {
  u8 t0 = BYTE_01;
  int t1 = t0 & -16;
  int t2 = a0 & 15;
  int t3 = t1 | t2;
  BYTE_01 = t3;
  return t3;
}
