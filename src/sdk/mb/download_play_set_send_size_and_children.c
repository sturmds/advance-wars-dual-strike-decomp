#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

extern DlPlayStationArea *data_022aced8;

int func_0211c4c4(u32 sendSize, u32 unknown, u32 maxChildren);

/* Sets the MP send size and the number of children, unless the parent already
 * started or the sizes do not fit. Returns 1 when they were set. */
int DownloadPlay_SetSendSizeAndChildren(u16 sendSize, u16 maxChildren)
{
    OSIntrMode enabled = OS_DisableInterrupts();

    if (data_022aced8->unk_50d) {
        OS_RestoreInterrupts(enabled);
        return 0;
    }
    if (!func_0211c4c4(sendSize, 8, maxChildren)) {
        OS_RestoreInterrupts(enabled);
        return 0;
    }
    data_022aced8->maxChildren = maxChildren;
    data_022aced8->sendSize = sendSize;
    data_022aced8->unk_502 = 8;
    OS_RestoreInterrupts(enabled);
    return 1;
}
