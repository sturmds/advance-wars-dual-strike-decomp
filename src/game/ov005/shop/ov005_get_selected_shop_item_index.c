#include "types.h"
#include "game/Unk_ov005_0236bbb0.h"

int Ov005_GetSelectedShopItemIndex(Unk_ov005_0236bbb0* work) {
  return work->cursorRow + work->page * 6;
}
