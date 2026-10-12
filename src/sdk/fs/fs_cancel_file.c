#include "sdk/fs.h"
#include "sdk/os.h"

void FS_CancelFile(FSFile* file) {
  int state = OS_DisableInterrupts();

  if (FS_IsBusy(file)) {
    file->flags |= FS_FILE_STATUS_CANCEL;
    file->arc->flags |= ARCHIVE_STATE_CANCEL_ASKED;
  }
  OS_RestoreInterrupts(state);
}
