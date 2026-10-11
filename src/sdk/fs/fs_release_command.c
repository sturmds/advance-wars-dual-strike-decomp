#include "sdk/fs.h"
#include "sdk/os.h"

void FsFile_DetachFromList(FSFile *file);
void func_0210d550(u32 *queue);

/* Ends the command running on a file: takes it off the archive's list,
 * clears the busy bits, stores the result and wakes the threads waiting on it. */
void FSi_ReleaseCommand(FSFile *file, int result)
{
    int irqState = OS_DisableInterrupts();

    FsFile_DetachFromList(file);
    file->flags &= ~FILE_HANDLE_BUSY_BITS;
    file->error = result;
    func_0210d550(&file->queue);
    OS_RestoreInterrupts(irqState);
}
