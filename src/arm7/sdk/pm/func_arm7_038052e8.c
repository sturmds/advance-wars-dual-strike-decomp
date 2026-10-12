#include "types.h"

int func_arm7_038052e8(int a0) {
  *((u16*)0x40001c0) = 0x8002 | (a0 << 11);
  return 0x40001c0;
}
