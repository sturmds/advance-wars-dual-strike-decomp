#include "types.h"
#include "game/co_select_screen.h"

void BuildCoPickListRepeated(void);
static u8 data_022a7294;
u8 gCoPickList[CO_LIST_SIZE]; /* size: up to the next symbol */

void* func_020e33e4(void) {
  BuildCoPickListRepeated();
  data_022a7294 = 0;
  return &data_022a7294;
}
