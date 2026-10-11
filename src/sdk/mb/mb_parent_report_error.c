#include "types.h"
#include "sdk/mb.h"

extern DlPlayStationArea *data_022aced8;

/* Passes a failed WM call to the parent's error callback. Error code 0 (success)
 * and 2 (still running) are not reported. */
void MbParent_ReportError(u16 apiid, int errcode)
{
    DlPlayFailedCall error;

    if (errcode == 2 || errcode == 0) {
        return;
    }
    error.apiid = apiid;
    error.errcode = errcode;
    data_022aced8->errorCallback(DLPLAY_EVENT_RADIO_FAULT, &error);
}
