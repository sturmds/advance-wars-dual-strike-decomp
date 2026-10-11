#include "sdk/fs.h"

BOOL FS_SeekFile(FSFile *file, s32 offset, int origin)
{
    /* offset becomes the absolute position (needed to match) */
    switch (origin) {
    case FS_SEEK_SET:
        offset += file->top;
        break;
    case FS_SEEK_CUR:
        offset += file->pos;
        break;
    case FS_SEEK_END:
        offset += file->bottom;
        break;
    default:
        return FALSE;
    }
    if (offset < (s32)file->top) {
        offset = file->top;
    }
    if (offset > (s32)file->bottom) {
        offset = file->bottom;
    }
    file->pos = offset;
    return TRUE;
}
