#include "sdk/fs.h"
#include "sdk/os.h"

void OS_SleepThread(OSThreadQueue *queue);

/* Default archive commands, indexed by ARCHIVE_OP_*. */
extern int (*const gArchiveFallbackCommands[])(FSFile *file);

static inline BOOL FileHandle_IsBlocking(FSFile *file)
{
    return (file->flags & FS_FILE_STATUS_SYNC) ? TRUE : FALSE;
}

static inline BOOL Archive_CallerBlocked(FSArchive *arc)
{
    return (arc->flags & ARCHIVE_STATE_CALLER_BLOCKED) ? TRUE : FALSE;
}

/* Runs one command on a file. The archive's own handler gets it first when
 * the archive asked for that command; otherwise, or when the handler passes,
 * the default command runs. A synchronous caller sleeps through an
 * asynchronous result; an asynchronous one gets the file released here. */
int FSi_TranslateCommand(FSFile *file, u32 command)
{
    int result;
    FSArchive *arc = file->arc;
    u32 bit = 1 << command;

    /* needed to match: each "!= FALSE" keeps the 0/1 value and its compare */
    if (FileHandle_IsBlocking(file) != FALSE) {
        arc->flags |= ARCHIVE_STATE_CALLER_BLOCKED;
    } else {
        arc->flags |= ARCHIVE_STATE_AWAITING_DRIVER;
    }

    /* tableBuffer and tableBufferSize hold the archive's handler and the
     * mask of commands it takes */
    if (arc->tableBufferSize & bit) {
        result = ((ArchiveCommandHook)arc->tableBuffer)(file, command);
        switch (result) {
        case FS_RESULT_SUCCESS:
        case FS_RESULT_FAILURE:
        case FS_RESULT_UNSUPPORTED:
            file->error = result;
            break;
        case FS_RESULT_PROC_UNKNOWN:
            arc->tableBufferSize &= ~bit;
            result = FS_RESULT_PROC_DEFAULT;
            break;
        }
    } else {
        result = FS_RESULT_PROC_DEFAULT;
    }
    if (result == FS_RESULT_PROC_DEFAULT) {
        result = gArchiveFallbackCommands[command](file);
    }

    if (result == FS_RESULT_PROC_ASYNC) {
        if (FileHandle_IsBlocking(file) != FALSE) {
            int irqState = OS_DisableInterrupts();

            while (Archive_CallerBlocked(arc) != FALSE) {
                OS_SleepThread(&arc->queue);
            }
            result = file->error;
            OS_RestoreInterrupts(irqState);
        }
    } else if (FileHandle_IsBlocking(file) != FALSE) {
        arc->flags &= ~ARCHIVE_STATE_CALLER_BLOCKED;
        file->error = result;
    } else {
        arc->flags &= ~ARCHIVE_STATE_AWAITING_DRIVER;
        FSi_ReleaseCommand(file, result);
    }
    return result;
}
