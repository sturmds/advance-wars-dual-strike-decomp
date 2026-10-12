#include "types.h"

extern int data_02167ea4[];
extern int data_02167ed0[];

/* Same table as MapObject_GetDrawOffsetX, the y of the pair. */
int MapObject_GetDrawOffsetY(int kind, int variant) {
  int* table;

  switch (kind) {
    case 3:
    default:
      table = data_02167ea4;
      break;
    case 4:
      table = data_02167ed0;
      break;
  }
  return table[variant * 2 + 1];
}
