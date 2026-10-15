#include "types.h"
#include "game/co_select_screen.h"

void CoSelect_ApplyPicks(void);
int Mem_Free(void* block); /* returns nothing; the caller passes r0 on */

int func_02042da4(void) {
  CoSelect_ApplyPicks();
  return Mem_Free(data_0229c73c);
}
