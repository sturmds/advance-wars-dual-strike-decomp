#include "sdk/fs.h"
#include "sdk/os.h"

void OS_SleepThread(OSThreadQueue* queue);
BOOL FSi_ExecuteSyncCommand(FSFile* file);

BOOL FS_WaitAsync(FSFile* file) {
  BOOL runHere = FALSE;
  int state = OS_DisableInterrupts();

  if (FS_IsBusy(file)) {
    /* A file the archive is not running yet is run by this thread once it wakes. */
    runHere = (file->arc->command != file);
    if (runHere) {
      file->flags |= FS_FILE_STATUS_SYNC;
    }
    do {
      OS_SleepThread(&file->queue);
    } while (FS_IsBusy(file));
  }
  OS_RestoreInterrupts(state);
  if (runHere) {
    return FSi_ExecuteSyncCommand(file);
  }
  return file->error == FS_RESULT_SUCCESS;
}
