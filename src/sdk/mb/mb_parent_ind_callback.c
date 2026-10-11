#include "types.h"
#include "sdk/mb.h"
#include "sdk/wm.h"

extern DlPlayStationArea *data_022aced8;

/* Port callback: tells errorCallback when data arrives on the MB port. Errors and
 * connect/disconnect events are ignored. */
void MbParent_PortCallback(RadioNotice *ind)
{
    if (ind->errcode != WM_ERRCODE_SUCCESS) {
        return;
    }
    switch (ind->state) {
    case WM_STATECODE_CONNECTED:
    case WM_STATECODE_DISCONNECTED:
        break;
    case WM_STATECODE_PORT_RECV:
        data_022aced8->errorCallback(DLPLAY_EVENT_PORT_DATA, ind);
        break;
    }
}
