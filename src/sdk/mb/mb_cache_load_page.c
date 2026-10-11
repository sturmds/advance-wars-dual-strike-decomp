#include "types.h"
#include "sdk/mb.h"
#include "sdk/fs.h"
#include "sdk/os.h"

/* Reads one cache page from the parent's own ROM. Stops the console when the
 * read comes up short. */
void MbCache_LoadPage(DlPlayWorkerJob *task)
{
    DlPlayHeldProgram *cache = task->cache;
    DlPlayHeldBlock *page = task->page;
    FSFile file;
    u32 address;

    FS_InitFile(&file);
    address = page->address;
    if (FS_OpenFileDirect(&file, FS_FindArchive(cache->archiveName, cache->archiveNameLength), address, address + page->size, -1)) {
        if (page->size == FS_ReadFile(&file, page->buffer, page->size)) {
            page->state = DLPLAY_HELD_BLOCK_USABLE;
        }
        FS_CloseFile(&file);
    }
    if (page->state != DLPLAY_HELD_BLOCK_USABLE) {
        OS_Terminate();
    }
}
