#include "sdk/fs.h"

extern u32 data_022ab83c;

void func_02115400(u16 lockId);
void func_02115420(u16 lockId);

/* Command handler of the ROM archive. It takes and gives back the card
 * lock when the archive becomes active or idle, and refuses writes. */
int FSi_RomArchiveProc(FSFile* file, int command) {
  switch (command) {
    case FS_COMMAND_ACTIVATE:
      func_02115420(data_022ab83c);
      return FS_RESULT_SUCCESS;
    case FS_COMMAND_IDLE:
      func_02115400(data_022ab83c);
      return FS_RESULT_SUCCESS;
    case ARCHIVE_OP_COPY_IN:
      return FS_RESULT_UNSUPPORTED;
    default:
      return FS_RESULT_PROC_UNKNOWN;
  }
}
