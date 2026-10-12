#include "types.h"
#include "game/unlocks.h"

extern u8 data_022a7f84[];

u8 func_02101904(u32 coId) {
  return (1 << (coId & 7)) & data_022a7f84[coId >> 3];
}
