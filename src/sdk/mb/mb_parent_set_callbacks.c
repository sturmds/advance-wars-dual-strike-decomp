#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

void MbParent_ReportError(u16 apiid, int errcode);
void func_0211d05c(void *arg);
int WM_SetLifeTime(WMCallbackFunc callback, u16 tableNumber, u16 camLifeTime, u16 frameLifeTime, u16 mpLifeTime);

/* Sends WM indications to the parent's state callback (func_0211d05c) and sets the
 * WM life times: 0xffff is every table entry, 40 and 5 are in WM's units. */
void MbParent_SetIndAndLifeTime(void)
{
    MbParent_ReportError(WM_APIID_INDICATION, WM_SetIndCallback(func_0211d05c));
    MbParent_ReportError(WM_APIID_SET_LIFETIME, WM_SetLifeTime(func_0211d05c, 0xffff, 40, 5, 40));
}
