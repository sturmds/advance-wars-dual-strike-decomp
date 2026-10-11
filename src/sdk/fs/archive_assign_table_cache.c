#include "sdk/fs.h"

/* Sets the buffer the archive caches its tables in. A missing buffer and a
 * size of 0 both mean no cache. */
void Archive_AssignTableCache(FSArchive *arc, void *buffer, u32 size)
{
    if (size == 0) {
        buffer = NULL;
    } else if (buffer == NULL) {
        size = 0;
    }
    arc->tableBuffer = buffer;
    arc->tableBufferSize = size;
}
