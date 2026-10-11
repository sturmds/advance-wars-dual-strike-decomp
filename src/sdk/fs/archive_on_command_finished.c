#include "sdk/fs.h"
#include "sdk/os.h"

FSFile *func_0211232c(FSArchive *arc);   /* picks the next command to run */
void func_02112290(FSFile *file);        /* runs commands until one answers "later" */
void OS_WakeupThread(OSThreadQueue *queue);

static inline BOOL Archive_AwaitsDriver(FSArchive *arc)
{
    return (arc->flags & ARCHIVE_STATE_AWAITING_DRIVER) ? TRUE : FALSE;
}

/* Called by an archive when a command that answered "later" ends. Releases
 * the command, then starts the next waiting one. A caller that sleeps on a
 * synchronous command is woken instead. */
void Archive_OnCommandFinished(FSArchive *arc, int result)
{
    /* needed to match: the compare with FALSE and the local for the running
     * command decide the register use */
    if (Archive_AwaitsDriver(arc) != FALSE) {
        FSFile *running = arc->command;
        FSFile *next;

        arc->flags &= ~ARCHIVE_STATE_AWAITING_DRIVER;
        FSi_ReleaseCommand(running, result);
        next = func_0211232c(arc);
        if (next != NULL) {
            func_02112290(next);
        }
    } else {
        FSFile *file = arc->command;
        int irqState = OS_DisableInterrupts();

        file->error = result;
        arc->flags &= ~ARCHIVE_STATE_CALLER_BLOCKED;
        OS_WakeupThread(&arc->queue);
        OS_RestoreInterrupts(irqState);
    }
}
