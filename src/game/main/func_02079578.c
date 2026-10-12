#include "types.h"

extern u32 gWirelessState;
extern void Wireless_OnStepDone(void);

BOOL func_02079578(void) {
  if (WM_Disable(Wireless_OnStepDone) != 2) {
    gWirelessState = 1;
    return FALSE;
  }
  return TRUE;
}
