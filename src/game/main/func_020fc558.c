#include "types.h"

extern int data_022a7e68;
extern void func_020fc594(void);

BOOL func_020fc558(void) {
  if (WM_PowerOff(func_020fc594) != 2) {
    data_022a7e68 = 6;
    return FALSE;
  }
  return TRUE;
}
