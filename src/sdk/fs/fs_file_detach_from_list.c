#include "sdk/fs.h"

/* Takes a file out of its list and clears its own links. */
void FsFile_DetachFromList(FSFile *file)
{
    FSFile *prev = file->prev;
    FSFile *next = file->next;

    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }
    file->prev = NULL;
    file->next = file->prev;
}
