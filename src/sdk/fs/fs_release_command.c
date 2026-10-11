#include "sdk/fs.h"
#include "sdk/os.h"

void FsFile_DetachFromList(FSFile *file);
void OS_WakeupThread(OSThreadQueue *queue);

/* Ends the command running on a file: takes it off the archive's list,
 * clears the busy bits, stores the result and wakes the threads waiting on it. */
void FSi_ReleaseCommand(FSFile *file, int result)
{
    int irqState = OS_DisableInterrupts();

    FsFile_DetachFromList(file);
    file->flags &= ~FILE_HANDLE_BUSY_BITS;
    file->error = result;
    OS_WakeupThread(&file->queue);
    OS_RestoreInterrupts(irqState);
}
