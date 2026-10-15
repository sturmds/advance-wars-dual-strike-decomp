#include "types.h"
#include "game/Unk_02080440.h"

BOOL MapView_FindScroll(int* x, int* y);

#define STATE_50 (data_027c027c->unk_50[data_027c0284])

/* Takes a position through MapView_FindScroll (which reads x and y by address, as a
 * variadic call), stores it times 16 in the map state at unk_1e and unk_20, then runs
 * the callback of the state's unk_50 record if there is one. */
void func_020f53e8(int x, int y, ...) {
  if (!MapView_FindScroll(&x, &y)) {
    return;
  }
  MAP_STATE->unk_1c4->scrollX = x << 4;
  MAP_STATE->unk_1c4->scrollY = y << 4;
  if (STATE_50->callback == NULL) {
    return;
  }
  STATE_50->callback();
}
