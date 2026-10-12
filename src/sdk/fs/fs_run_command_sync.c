#include "sdk/fs.h"

int FSi_TranslateCommand(FSFile* file,
                         u32 command); /* runs one command through the table */
FSFile* FSi_NextCommand(FSArchive* arc); /* picks the next command to run */
void FSi_ExecuteAsyncCommand(FSFile* file);

/* Runs the file's command, releases it with the result, then starts the
 * next waiting command of the archive. Returns TRUE when the command
 * succeeded. */
BOOL FSi_ExecuteSyncCommand(FSFile* file) {
  FSFile* next;

  FSi_ReleaseCommand(file, FSi_TranslateCommand(file, file->command));
  next = FSi_NextCommand(file->arc);
  if (next != NULL) {
    FSi_ExecuteAsyncCommand(next);
  }
  return file->error == FS_RESULT_SUCCESS;
}
