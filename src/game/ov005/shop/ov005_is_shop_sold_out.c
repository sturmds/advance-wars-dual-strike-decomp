#include "types.h"
#include "game/Unk_ov005_0236bbb0.h"

extern Unk_ov005_0236bbb0* data_ov005_0236bbb0;

/* TRUE when the shop has no item left for sale. */
BOOL Ov005_IsShopSoldOut(void) {
  return (data_ov005_0236bbb0->itemCount == 0) ? TRUE : FALSE;
}
