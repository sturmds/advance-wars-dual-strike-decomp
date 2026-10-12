#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

extern DlPlayStationArea *data_022aced8;

void MbParent_ReportError(u16 apiid, int errcode);

/* Calls WM_Reset with the parent's WM callback, reports a failure and
 * returns the WM error code, with "still running" counted as success. */
int MbParent_ResetWm(void)
{
    int result = WM_Reset(data_022aced8->wmCallback);

    MbParent_ReportError(WM_APIID_RESET, result);
    if (result == WM_ERRCODE_OPERATING) {
        result = WM_ERRCODE_SUCCESS;
    }
    return result;
}
