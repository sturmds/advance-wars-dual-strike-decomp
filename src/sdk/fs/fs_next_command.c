#include "sdk/fs.h"
#include "sdk/os.h"

void OS_WakeupThread(OSThreadQueue *queue);

static inline BOOL FileHandle_IsBlocking(FSFile *file)
{
    return (file->flags & FS_FILE_STATUS_SYNC) ? TRUE : FALSE;
}

static inline BOOL FileHandle_CancelAsked(FSFile *file)
{
    return (file->flags & FS_FILE_STATUS_CANCEL) ? TRUE : FALSE;
}

/* needed to match: the original loads flags again for every test, as if
 * the field were volatile */
static inline u32 Archive_ReadStateVolatile(FSArchive *arc)
{
    return *(volatile u32 *)&arc->flags;
}

static inline BOOL Archive_CancelAsked(FSArchive *arc)
{
    return (Archive_ReadStateVolatile(arc) & ARCHIVE_STATE_CANCEL_ASKED) ? TRUE : FALSE;
}

static inline BOOL Archive_PauseAsked(FSArchive *arc)
{
    return (Archive_ReadStateVolatile(arc) & ARCHIVE_STATE_PAUSE_ASKED) ? TRUE : FALSE;
}

static inline BOOL Archive_IsPaused(FSArchive *arc)
{
    return (Archive_ReadStateVolatile(arc) & ARCHIVE_STATE_PAUSED) ? TRUE : FALSE;
}

static inline BOOL Archive_IsBusy(FSArchive *arc)
{
    return (Archive_ReadStateVolatile(arc) & ARCHIVE_STATE_BUSY) ? TRUE : FALSE;
}

/* Picks the archive's next command after one ends. Releases canceled files
 * first. Returns the file at the head of the queue when its command is
 * asynchronous, for the caller to run; wakes the waiting thread of a
 * synchronous one and returns NULL. With nothing to run, the archive goes
 * idle (and suspends if that was asked) and NULL is returned. */
FSFile *FSi_NextCommand(FSArchive *arc)
{
    int irqState = OS_DisableInterrupts();

    if (Archive_CancelAsked(arc) != FALSE) {
        FSFile *file;
        FSFile *next;

        arc->flags &= ~ARCHIVE_STATE_CANCEL_ASKED;
        for (file = arc->command; file != NULL; file = next) {
            next = file->next;
            if (FileHandle_CancelAsked(file) != FALSE) {
                if (arc->command == file) {
                    arc->command = next;
                }
                FSi_ReleaseCommand(file, FS_RESULT_CANCELED);
                if (next == NULL) {
                    next = arc->command;
                }
            }
        }
    }

    if (Archive_PauseAsked(arc) == FALSE && Archive_IsPaused(arc) == FALSE) {
        FSFile *file = arc->command;

        if (file != NULL) {
            /* needed to match: the ternary keeps both 0/1 steps */
            BOOL starting = (Archive_IsBusy(arc) != FALSE) ? FALSE : TRUE;

            if (starting) {
                arc->flags |= ARCHIVE_STATE_BUSY;
            }
            OS_RestoreInterrupts(irqState);
            /* the handler and its mask sit in tableBuffer and tableBufferSize */
            if (starting && (arc->tableBufferSize & (1 << FS_COMMAND_ACTIVATE))) {
                ((ArchiveCommandHook)arc->tableBuffer)(file, FS_COMMAND_ACTIVATE);
            }
            if (FileHandle_IsBlocking(file) == FALSE) {
                return file;
            }
            OS_WakeupThread(&file->queue);
            return NULL;
        }
    }

    if (Archive_IsBusy(arc) != FALSE) {
        arc->flags &= ~ARCHIVE_STATE_BUSY;
        if (arc->tableBufferSize & (1 << FS_COMMAND_IDLE)) {
            FSFile idle;

            FS_InitFile(&idle);
            idle.arc = arc;
            ((ArchiveCommandHook)arc->tableBuffer)(&idle, FS_COMMAND_IDLE);
        }
    }
    if (Archive_PauseAsked(arc) != FALSE) {
        arc->flags &= ~ARCHIVE_STATE_PAUSE_ASKED;
        arc->flags |= ARCHIVE_STATE_PAUSED;
        /* queue.tail is a second queue: threads waiting for the suspend */
        OS_WakeupThread((OSThreadQueue *)&arc->queue.tail);
    }
    OS_RestoreInterrupts(irqState);
    return NULL;
}
