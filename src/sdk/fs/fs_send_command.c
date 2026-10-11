#include "sdk/fs.h"
#include "sdk/os.h"

void OS_SleepThread(OSThreadQueue *queue);
BOOL FSi_ExecuteSyncCommand(FSFile *file);
void FSi_ExecuteAsyncCommand(FSFile *file);

static inline BOOL FileHandle_IsBlocking(FSFile *file)
{
    return (file->flags & FS_FILE_STATUS_SYNC) ? TRUE : FALSE;
}

static inline BOOL Archive_IsPaused(FSArchive *arc)
{
    return (arc->flags & ARCHIVE_STATE_PAUSED) ? TRUE : FALSE;
}

/* needed to match: the volatile read makes flags load again after the
 * suspended test */
static inline BOOL Archive_IsBusy(FSArchive *arc)
{
    return (*(volatile u32 *)&arc->flags & ARCHIVE_STATE_BUSY) ? TRUE : FALSE;
}

/* Queues a command on the file's archive. When the archive is idle the
 * command starts at once; otherwise a synchronous caller sleeps until its
 * file reaches the head of the queue. A synchronous command returns whether
 * it succeeded; an asynchronous one returns TRUE once queued. Fails when the
 * archive is being unloaded. */
BOOL FSi_SendCommand(FSFile *file, u32 command)
{
    FSArchive *arc = file->arc;
    u32 bit = 1 << command;
    int irqState;

    file->command = command;
    file->error = FS_RESULT_BUSY;
    file->flags |= FILE_HANDLE_QUEUED;

    irqState = OS_DisableInterrupts();
    if (arc->flags & ARCHIVE_STATE_TEARING_DOWN) {
        FSi_ReleaseCommand(file, FS_RESULT_CANCELED);
        OS_RestoreInterrupts(irqState);
        return FALSE;
    }
    if (bit & ARCHIVE_OP_BLOCKING_SET) {
        file->flags |= FS_FILE_STATUS_SYNC;
    }
    /* unk_14 and the field after it link the archive's queue of files, laid
     * out as the prev and next of an FSFile */
    FSi_AppendToList(file, (FSFile *)&arc->unk_14);
    /* needed to match: the compares with FALSE */
    if (Archive_IsPaused(arc) == FALSE && Archive_IsBusy(arc) == FALSE) {
        arc->flags |= ARCHIVE_STATE_BUSY;
        OS_RestoreInterrupts(irqState);
        /* the handler and its mask sit in tableBuffer and tableBufferSize */
        if (arc->tableBufferSize & (1 << FS_COMMAND_ACTIVATE)) {
            ((ArchiveCommandHook)arc->tableBuffer)(file, FS_COMMAND_ACTIVATE);
        }
        if (FileHandle_IsBlocking(file) == FALSE) {
            FSi_ExecuteAsyncCommand(file);
            return TRUE;
        }
    } else if (FileHandle_IsBlocking(file) == FALSE) {
        OS_RestoreInterrupts(irqState);
        return TRUE;
    } else {
        do {
            OS_SleepThread(&file->queue);
        } while (file != arc->command);
        OS_RestoreInterrupts(irqState);
    }
    return FSi_ExecuteSyncCommand(file);
}
