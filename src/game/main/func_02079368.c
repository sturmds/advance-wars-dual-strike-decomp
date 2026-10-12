#include "types.h"

extern u32 gWirelessState;
extern void Wireless_OnEventNextStep(void);

BOOL func_02079368(void) {
  if (WM_PowerOff(Wireless_OnEventNextStep) != 2) {
    gWirelessState = 1;
    return FALSE;
  }
  return TRUE;
}
