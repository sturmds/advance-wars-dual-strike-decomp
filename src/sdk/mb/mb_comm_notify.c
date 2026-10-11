#include "types.h"
#include "sdk/mb.h"

extern DlPlayHostLinkArea *data_022aced4;

/* Calls the application's comm callback, if it set one. */
void MbComm_NotifyState(u32 aid, u32 state, void *arg)
{
    if (data_022aced4->callback != NULL) {
        data_022aced4->callback(aid, state, arg);
    }
}
