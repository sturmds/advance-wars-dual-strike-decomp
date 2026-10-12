#include "sdk/fs.h"

/* Puts a file in the unused state: on no list, no archive, no command. */
void FS_InitFile(FSFile* file) {
  file->prev = NULL;
  file->next = file->prev;
  file->queue.head = 0;
  file->arc = NULL;
  file->command = FS_COMMAND_INVALID;
  file->flags = 0;
}
