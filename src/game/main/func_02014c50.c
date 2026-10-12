#include "types.h"

extern u8 gEventSkipState;

void* func_02014c50(void) {
  gEventSkipState = 0;
  return &gEventSkipState;
}
