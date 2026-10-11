#include "types.h"
#include "sdk/mb.h"

extern DlPlayStationArea *data_022aced8;

/* Nonzero when MP is running, no send is queued and the parent is not ending.
 * The u8/s8 locals are needed to match. */
int MbParent_CanSendMpData(void)
{
    DlPlayStationArea *work;
    u8 ready;
    u8 result;
    s8 idle;

    result = 0;
    ready = 0;
    idle = 0;
    work = data_022aced8;

    if (work->unk_528 == 1) {
        if (work->sendQueued == 0) {
            idle = 1;
        }
    }
    if (idle) {
        if (work->unk_526 == 0) {
            ready = 1;
        }
    }
    if (ready) {
        if (work->unk_52a != 0) {
            result = 1;
        }
    }
    return result;
}
