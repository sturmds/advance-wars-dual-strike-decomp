#include "types.h"

u8* func_020193e0(int);

void BattleSave_SetRecordFlag(int a0, int a1) {
  u8* t0 = func_020193e0((s8)a0);
  *t0 = a1;
}
