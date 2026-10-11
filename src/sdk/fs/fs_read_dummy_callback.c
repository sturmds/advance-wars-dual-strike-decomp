#include "sdk/fs.h"

/* Read routine of an archive with no files: every read fails. */
int FSi_ReadDummyCallback(FSArchive *arc, void *dest, u32 src, u32 len)
{
    return FS_RESULT_FAILURE;
}
