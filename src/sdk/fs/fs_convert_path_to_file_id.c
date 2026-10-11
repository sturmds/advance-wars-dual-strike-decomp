#include "sdk/fs.h"

BOOL FS_ConvertPathToFileID(FSFileID *fileId, const char *path)
{
    FSFile file;

    FS_InitFile(&file);
    return func_02112a54(&file, path, fileId, NULL) != 0;
}
