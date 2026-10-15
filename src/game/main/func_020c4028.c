#include "types.h"
#include "game/Unk_02080440.h"

void MapCursor_MoveByKeys(void);
void MapCursor_UpdateFromTouch(void);

void func_020c4028(void) {
  if ((MAP_STATE->unk_1c4->scrollFlags & 0xf) != 0) {
    return;
  }
  MapCursor_MoveByKeys();
  MapCursor_UpdateFromTouch();
}
