#include "types.h"
#include "game/Unk_02080440.h"

void MapCursor_MoveByKeysFree(void);
void MapCursor_UpdateFromTouchFree(void);

void MapCursor_UpdateFree(void) {
  if ((MAP_STATE->unk_1c4->scrollFlags & 0xf) != 0) {
    return;
  }
  MapCursor_MoveByKeysFree();
  MapCursor_UpdateFromTouchFree();
}
