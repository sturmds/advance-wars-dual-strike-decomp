#include "types.h"

BOOL Map_AddTileOffset(int* x, int* y, int list, int index);
void Unit_DamageAtTile(int x, int y, int arg);
void func_020c4018(void);

/* Takes damage off the units on every tile of offset list `list` around (x, y). */
void Units_DamageAroundTile(int x, int y, int damage, int list) {
  int pos[2];
  int index = 0;

  do {
    pos[0] = x;
    pos[1] = y;
    if (!Map_AddTileOffset(&pos[0], &pos[1], list, index++)) {
      break;
    }
    Unit_DamageAtTile(pos[0], pos[1], damage);
  } while (1);
  func_020c4018();
}
