#include "types.h"
#include "sdk/wm.h"

extern int data_022a7e68;
BOOL func_020fc634(void);

void func_020fc594(WMCallback* arg) {
  if (arg->errcode != 0) {
    data_022a7e68 = 6;
    return;
  }
  func_020fc634();
}
