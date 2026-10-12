#include "sdk/fs.h"

/* Archive command: opens a file from the top, bottom and id the caller put
 * in the command arguments. */
int FSi_OpenFileDirectCommand(FSFile* file) {
  file->top = file->arg.openFileDirect.top;
  file->pos = file->arg.openFileDirect.top;
  file->bottom = file->arg.openFileDirect.bottom;
  file->id = file->arg.openFileDirect.id;
  return FS_RESULT_SUCCESS;
}
