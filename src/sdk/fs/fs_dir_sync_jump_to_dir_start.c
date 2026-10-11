#include "sdk/fs.h"

int FSi_TranslateCommand(FSFile *file, u32 command);

/* Marks the file busy and runs the seek-directory command at the
 * start of directory dirId. */
int FsDir_SyncJumpToDirStart(FSFile *file, int dirId)
{
    file->flags |= FS_FILE_STATUS_BUSY;
    file->arg.seekDir.pos.arc = file->arc;
    file->arg.seekDir.pos.pos = 0;
    file->arg.seekDir.pos.index = 0;
    file->arg.seekDir.pos.ownId = dirId;
    return FSi_TranslateCommand(file, ARCHIVE_OP_SET_DIR_POS);
}
