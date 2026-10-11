#include "sdk/fs.h"

extern FSDirPos data_022ab82c;   /* current directory (guess: gWorkingDirPos) */

BOOL FS_ChangeDir(const char *path)
{
    FSDirPos dir;
    FSFile file;

    FS_InitFile(&file);
    if (!func_02112a54(&file, path, NULL, &dir)) {
        return FALSE;
    }
    data_022ab82c = dir;
    return TRUE;
}
