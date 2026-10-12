#include "types.h"

int func_ov006_0235b784(int x, int y);

/* 1 if func_ov006_0235b784 accepts all nine tiles around (x, y). */
int Ov006_IsAreaClear(int x, int y) {
  int col;
  int row;

  for (row = y - 1; row <= y + 1; row++) {
    for (col = x - 1; col <= x + 1; col++) {
      if (func_ov006_0235b784(col, row) == 0) {
        return 0;
      }
    }
  }
  return 1;
}
