#include "types.h"
#include "game/Unk_ov005_0236bbb0.h"

extern Unk_ov005_0236bbb0* data_ov005_0236bbb0;

/* Touch button: turns to the next page like R. */
void Ov005_RequestShopNextPage(void) {
  data_ov005_0236bbb0->nextPageRequested = 1;
}
