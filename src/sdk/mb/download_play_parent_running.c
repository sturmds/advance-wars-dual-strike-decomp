#include "types.h"
#include "sdk/mb.h"

extern DlPlayStationArea *data_022aced8;

/* TRUE between MbParent_InitWm and the end of the session. */
BOOL DownloadPlay_ParentRunning(void)
{
    return (data_022aced8->started == 1) ? 1 : 0;
}
