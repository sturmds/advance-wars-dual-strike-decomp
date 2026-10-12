#include "types.h"

extern void* data_02297ee0;
extern u8 data_02297ee4[];

void* PopupMenu_ResetStack(void) {
  data_02297ee0 = data_02297ee4;
  return &data_02297ee0;
}
