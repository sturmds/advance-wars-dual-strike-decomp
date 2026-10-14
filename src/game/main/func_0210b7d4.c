#include "types.h"
#include "sdk/gx.h"

void MIi_CpuCopy16(int, int, int);

void func_0210b7d4(int a0) {
  MIi_CpuCopy16(a0, TOON_TABLE_REG_ADDR, TOON_TABLE_SIZE);
}
