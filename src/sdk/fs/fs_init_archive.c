#include "sdk/fs.h"

void MI_CpuFill8(void *dest, u32 value, u32 size);   /* looks like MI_CpuFill8 */

/* Clears the archive and its thread queue before it is loaded. */
void FS_InitArchive(FSArchive *arc)
{
    MI_CpuFill8(arc, 0, sizeof(FSArchive));
    arc->queue.head = 0;
    arc->queue.tail = 0;
}
