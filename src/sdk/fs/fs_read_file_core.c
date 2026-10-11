#include "sdk/fs.h"

/* Reads up to length bytes at the file's position. Without async it waits and
 * returns the bytes read, or -1 on error; with async it returns the length cut
 * to what is left in the file. */
s32 FSi_ReadFileCore(FSFile *file, void *dest, s32 length, BOOL async)
{
    u32 startPos = file->pos;
    s32 left = file->bottom - startPos;
    s32 requested = length;   /* needed to match: keeps the file pointer in r7 */

    file->arg.readFile.dest = dest;
    if (length > left) {
        length = left;
    }
    if (length < 0) {
        length = 0;
    }
    file->arg.readFile.requested = requested;
    file->arg.readFile.length = length;
    if (!async) {
        file->flags |= FS_FILE_STATUS_SYNC;
    }
    func_021120d4(file, ARCHIVE_OP_COPY_OUT);
    if (!async) {
        if (FS_WaitAsync(file)) {
            length = file->pos - startPos;
        } else {
            length = -1;
        }
    }
    return length;
}
