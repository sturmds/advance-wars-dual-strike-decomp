#include "types.h"
#include "game/unlocks.h"

extern u8 data_022a7f84[];

void func_021019a4(u32 coId, int value) {
  int bit = 1 << (coId & 7);
  data_022a7f84[coId >> 3] = (~bit & data_022a7f84[coId >> 3]) | (bit * value);
}
