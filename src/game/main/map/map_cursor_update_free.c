#include "types.h"
#include "game/Unk_02080440.h"

void MapCursor_MoveByKeysFree(void);
void MapCursor_UpdateFromTouchFree(void);
extern Unk_02080440* data_027c027c;
extern int data_027c0284;
#define STATE (data_027c027c->unk_48[data_027c0284])

void MapCursor_UpdateFree(void) {
  if ((STATE->unk_1c4->scrollFlags & 0xf) != 0) {
    return;
  }
  MapCursor_MoveByKeysFree();
  MapCursor_UpdateFromTouchFree();
}
