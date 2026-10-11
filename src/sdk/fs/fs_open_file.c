#include "sdk/fs.h"

BOOL func_02112988(FSFileID *fileId, const char *path);   /* path to file ID */
BOOL FS_OpenFileFast(FSFile *file, FSFileID fileId);        /* opens by file ID */

BOOL FS_OpenFile(FSFile *file, const char *path)
{
    FSFileID fileId;

    if (func_02112988(&fileId, path)) {
        if (FS_OpenFileFast(file, fileId)) {
            return TRUE;
        }
    }
    return FALSE;
}
