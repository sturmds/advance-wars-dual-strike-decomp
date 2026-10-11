#include "sdk/fs.h"

/* Write routine of the ROM archive: the cartridge cannot be written. */
int FSi_WriteDummyCallback(FSArchive *arc, const void *src, u32 dest, u32 len)
{
    return FS_RESULT_FAILURE;
}
