#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

void MbParent_ReportError(u16 apiid, int errcode);
void MbParent_WmCallback(void* arg);
int WM_SetLifeTime(WMCallbackFunc callback, u16 tableNumber, u16 camLifeTime,
                   u16 frameLifeTime, u16 mpLifeTime);

/* Sends WM indications to the parent's state callback (MbParent_WmCallback) and sets the
 * WM life times: 0xffff is every table entry, 40 and 5 are in WM's units. */
void MbParent_SetIndAndLifeTime(void) {
  MbParent_ReportError(WM_APIID_INDICATION,
                       WM_SetIndCallback(MbParent_WmCallback));
  MbParent_ReportError(WM_APIID_SET_LIFETIME,
                       WM_SetLifeTime(MbParent_WmCallback, 0xffff, 40, 5, 40));
}
