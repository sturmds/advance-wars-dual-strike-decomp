#include "types.h"
#include "sdk/gx.h"

void Bg2_SetPriority(int priority) {
  *REG_BG2CNT = (*REG_BG2CNT & ~3) | priority;
}
