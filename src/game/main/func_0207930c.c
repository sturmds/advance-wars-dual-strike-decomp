#include "types.h"

extern u32 gWirelessState;

extern int WM_SetIndCallback(void (*callback)(void));
extern void Wireless_OnIndication(void);

/* Returns TRUE when the call returns 0; otherwise sets gWirelessState to 1 and returns FALSE. */
BOOL func_0207930c(void) {
  if (WM_SetIndCallback(Wireless_OnIndication) != 0) {
    gWirelessState = 1;
    return FALSE;
  }
  return TRUE;
}
