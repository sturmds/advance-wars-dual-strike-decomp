#include "types.h"

void BuildCoPickListRepeated(void);
extern u8 data_022a7294;

void* func_020e33e4(void) {
  BuildCoPickListRepeated();
  data_022a7294 = 0;
  return &data_022a7294;
}
