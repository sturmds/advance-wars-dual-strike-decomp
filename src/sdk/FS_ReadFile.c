#include "sdk/fs.h"

s32 FSi_ReadFileCore(FSFile *file, void *dest, s32 length, BOOL async);

s32 FS_ReadFile(FSFile *file, void *dest, s32 length)
{
    return FSi_ReadFileCore(file, dest, length, FALSE);
}
