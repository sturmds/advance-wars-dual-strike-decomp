#include "types.h"
#include "sdk/card_arm7.h"

u32 func_arm7_038028c8(u32 priority) {
  return CARD_SetThreadPriority(priority);
}
