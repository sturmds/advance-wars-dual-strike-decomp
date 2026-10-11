#include "sdk/fs.h"

void Mem_CopyBytes(const void *src, void *dest, u32 size);

int FSi_ReadMemCallback(FSArchive *arc, void *dest, u32 src, u32 len)
{
    Mem_CopyBytes((const void *)(arc->base + src), dest, len);
    return FS_RESULT_SUCCESS;
}
