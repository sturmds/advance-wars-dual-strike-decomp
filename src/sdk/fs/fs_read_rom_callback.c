#include "sdk/fs.h"

extern u32 data_022ab840;

void func_02112f40(FSArchive *arc);
void func_0211607c(u32 dmaNo, u32 src, void *dest, u32 len, void (*done)(FSArchive *), FSArchive *arc,
                   BOOL isAsync);

/* Read routine of the ROM archive: starts a card read and returns
 * FS_RESULT_PROC_ASYNC. func_02112f40 reports the end of the read. */
int FSi_ReadRomCallback(FSArchive *arc, void *dest, u32 src, u32 len)
{
    func_0211607c(data_022ab840, src, dest, len, func_02112f40, arc, TRUE);
    return FS_RESULT_PROC_ASYNC;
}
