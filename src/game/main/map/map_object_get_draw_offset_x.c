#include "types.h"

extern int data_02167ea4[];
extern int data_02167ed0[];

/* Tile offset added to the object's x when drawing it: pairs of (x, y) per
 * variant, one table for kind 4 and one for the others. */
int MapObject_GetDrawOffsetX(int kind, int variant) {
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
  return table[variant * 2];
}
