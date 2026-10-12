#include "types.h"
#include "sdk/wm.h"

extern int data_022a7e68;

void func_020fc60c(WMCallback* arg) {
  if (arg->errcode == 0) {
    data_022a7e68 = 1;
  } else {
    data_022a7e68 = 5;
  }
}
