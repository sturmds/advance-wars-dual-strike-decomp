#include "types.h"

extern int data_022a7e68;
extern void func_020fc60c(void);

BOOL func_020fc5d0(void) {
  if (WM_PowerOn(func_020fc60c) != 2) {
    data_022a7e68 = 5;
    return FALSE;
  }
  return TRUE;
}
