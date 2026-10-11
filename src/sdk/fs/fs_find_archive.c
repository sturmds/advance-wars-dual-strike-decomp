#include "sdk/fs.h"
#include "sdk/os.h"

extern FSArchive *data_022ab828;   /* first registered archive (guess: gArchiveChainHead) */

u32 func_021125f4(const char *name, int length);

FSArchive *FS_FindArchive(const char *name, int length)
{
    u32 packedName = func_021125f4(name, length);
    int state = OS_DisableInterrupts();
    FSArchive *arc = data_022ab828;

    while (arc != NULL && arc->name != packedName) {
        arc = arc->next;
    }
    OS_RestoreInterrupts(state);
    return arc;
}
