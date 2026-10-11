#include "sdk/fs.h"
#include "sdk/os.h"

extern FSArchive *data_022ab828;       /* first registered archive */
extern FSDirPos data_022ab82c;         /* current directory */

/* Adds the archive to the list of registered archives under a name of up to
 * four characters. The first archive also becomes the current directory.
 * Returns FALSE when an archive already has that name. */
BOOL FS_RegisterArchiveName(FSArchive *arc, const char *name, int nameLength)
{
    BOOL registered = FALSE;
    int irqState = OS_DisableInterrupts();

    if (FS_FindArchive(name, nameLength) == NULL) {
        if (data_022ab828 == NULL) {
            data_022ab828 = arc;
            data_022ab82c.arc = arc;
            data_022ab82c.pos = 0;
            data_022ab82c.index = 0;
            data_022ab82c.ownId = 0;
        } else {
            FSArchive *last = data_022ab828;

            while (last->next != NULL) {
                last = last->next;
            }
            last->next = arc;
            arc->prev = last;
        }
        arc->name = Archive_FoldNameIntoWord(name, nameLength);
        arc->flags |= ARCHIVE_STATE_IN_LIST;
        registered = TRUE;
    }
    OS_RestoreInterrupts(irqState);
    return registered;
}
