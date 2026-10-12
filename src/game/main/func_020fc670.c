#include "types.h"
#include "sdk/wm.h"

extern int data_022a7e68;

void func_020fc670(WMCallback* arg) {
  if (arg->errcode == 0) {
    data_022a7e68 = 0;
  } else {
    data_022a7e68 = 6;
  }
}
