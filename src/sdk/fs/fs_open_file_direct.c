#include "sdk/fs.h"

BOOL FS_OpenFileDirect(FSFile *file, FSArchive *arc, u32 top, u32 bottom, u32 index)
{
    file->arc = arc;
    file->arg.openFileDirect.id = index;
    file->arg.openFileDirect.top = top;
    file->arg.openFileDirect.bottom = bottom;
    if (!FSi_SendCommand(file, ARCHIVE_OP_OPEN_BY_EXTENT)) {
        return FALSE;
    }
    file->flags |= FS_FILE_STATUS_IS_FILE;
    file->flags &= ~FS_FILE_STATUS_IS_DIR;
    return TRUE;
}
