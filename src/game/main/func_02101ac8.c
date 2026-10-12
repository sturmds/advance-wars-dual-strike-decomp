#include "types.h"
#include "game/unlocks.h"

extern u8 data_022a7f58[];

void func_02101ac8(u32 coId, int value) {
  int bit = 1 << (coId & 7);
  data_022a7f58[coId >> 3] = (~bit & data_022a7f58[coId >> 3]) | (bit * value);
}
