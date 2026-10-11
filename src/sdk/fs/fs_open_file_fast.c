#include "sdk/fs.h"

BOOL FS_OpenFileFast(FSFile *file, FSFileID fileId)
{
    if (fileId.arc == NULL) {
        return FALSE;
    }
    file->arc = fileId.arc;
    *(FSFileID *)&file->arg.openFileFast = fileId;   /* the archive and file ID, as the command's argument */
    if (!FSi_SendCommand(file, ARCHIVE_OP_OPEN_BY_ID)) {
        return FALSE;
    }
    file->flags |= FS_FILE_STATUS_IS_FILE;
    file->flags &= ~FS_FILE_STATUS_IS_DIR;
    return TRUE;
}
