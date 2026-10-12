#include "types.h"
#include "game/Unk_ov005_0236bbb0.h"

extern Unk_ov005_0236bbb0* data_ov005_0236bbb0;

/* Touch button: turns to the previous page like L. */
void Ov005_RequestShopPrevPage(void) {
  data_ov005_0236bbb0->prevPageRequested = 1;
}
