#include "sdk/fs.h"

BOOL func_02112988(FSFileID *fileId, const char *path);   /* path to file ID */
BOOL func_021128b4(FSFile *file, FSFileID fileId);        /* opens by file ID */

BOOL FS_OpenFile(FSFile *file, const char *path)
{
    FSFileID fileId;

    if (func_02112988(&fileId, path)) {
        if (func_021128b4(file, fileId)) {
            return TRUE;
        }
    }
    return FALSE;
}
