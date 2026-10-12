#include "types.h"
#include "game/Unk_ov005_0236bbb0.h"

/* Items from the first row of the current page to the end of the list. */
int Ov005_GetShopItemsFromPage(Unk_ov005_0236bbb0* work) {
  return work->itemCount - work->page * 6;
}
