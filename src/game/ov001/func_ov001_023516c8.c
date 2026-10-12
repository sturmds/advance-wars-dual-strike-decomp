#include "types.h"

BOOL TimeLimit_GetRemaining(int* value);

BOOL func_ov001_023516c8(void) {
  int value;

  if (!TimeLimit_GetRemaining(&value)) {
    return 0;
  }
  if (value == 0) {
    return 1;
  }
  return 0;
}
