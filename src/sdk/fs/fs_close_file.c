#include "sdk/fs.h"

BOOL FS_CloseFile(FSFile* file) {
  if (!FSi_SendCommand(file, ARCHIVE_OP_RELEASE_HANDLE)) {
    return FALSE;
  }
  file->arc = NULL;
  file->command = FS_COMMAND_INVALID;
  file->flags &= ~(FS_FILE_STATUS_IS_FILE | FS_FILE_STATUS_IS_DIR);
  return TRUE;
}
