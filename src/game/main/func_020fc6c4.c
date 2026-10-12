#include "types.h"
#include "sdk/wm.h"

extern int data_022a7e68;
BOOL func_020fc5d0(void);

void func_020fc6c4(WMCallback* arg) {
  if (arg->errcode != 0) {
    data_022a7e68 = 5;
    return;
  }
  func_020fc5d0();
}
