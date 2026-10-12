#include "sdk/fs.h"
#include "sdk/os.h"

void OS_SleepThread(OSThreadQueue* queue); /* looks like OS_SleepThread */

static inline BOOL Archive_CallerBlocked(FSArchive* arc) {
  return (arc->flags & ARCHIVE_STATE_CALLER_BLOCKED) ? TRUE : FALSE;
}

/* Reads length bytes of the FAT or FNT from the archive into dest and moves
 * the cursor past them. When the archive answers "later", sleeps until the
 * archive reports the end of the command. */
void FSi_ReadTable(ArchiveTableCursor* cursor, void* dest, u32 length) {
  FSArchive* arc = cursor->arc;
  int result;

  arc->flags |= ARCHIVE_STATE_CALLER_BLOCKED;
  result = arc->readTable(arc, dest, cursor->pos, length);
  switch (result) {
    case FS_RESULT_SUCCESS:
    case FS_RESULT_FAILURE:
      arc->flags &= ~ARCHIVE_STATE_CALLER_BLOCKED;
      break;
    case FS_RESULT_PROC_ASYNC: {
      int irqState = OS_DisableInterrupts();

      /* needed to match: the compare with FALSE keeps the loop test at the bottom */
      while (Archive_CallerBlocked(arc) != FALSE) {
        OS_SleepThread(&arc->queue);
      }
      OS_RestoreInterrupts(irqState);
      break;
    }
  }
  cursor->pos += length;
}
