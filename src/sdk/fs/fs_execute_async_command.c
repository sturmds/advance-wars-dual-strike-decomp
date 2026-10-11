#include "sdk/fs.h"
#include "sdk/os.h"

void OS_WakeupThread(OSThreadQueue *queue);
int func_02111bc4(FSFile *file, u32 command);      /* runs one command through the table */
FSFile *func_0211232c(FSArchive *arc);             /* picks the next command to run */

void FSi_ExecuteAsyncCommand(FSFile *file)
{
    FSArchive *arc = file->arc;
    int state;
    BOOL isSync;

    if (file == NULL) {
        return;
    }
    do {
        state = OS_DisableInterrupts();
        isSync = FileHandle_HasWaiter(file);
        if (isSync) {
            /* A thread waits for this command: wake it. The original returns with
             * interrupts still disabled here. */
            OS_WakeupThread(&file->queue);
            return;
        }
        file->flags |= FS_FILE_STATUS_ASYNC;
        OS_RestoreInterrupts(state);
        if (func_02111bc4(file, file->command) == FS_RESULT_PROC_ASYNC) {
            return;
        }
        file = func_0211232c(arc);
    } while (file != NULL);
}
