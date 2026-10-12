#include "types.h"

/* Stores a value in *value and returns 1 when it has one, else 0. */
BOOL TimeLimit_GetRemaining(int* value);

BOOL func_ov001_02350900(void) {
  int value;

  if (!TimeLimit_GetRemaining(&value)) {
    return 0;
  }
  if (value == 0) {
    return 1;
  }
  return 0;
}
