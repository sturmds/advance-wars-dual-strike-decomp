#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

void Mem_CopyBytes(const void *src, void *dst, u32 size);

/* Copies size bytes at program address to dst from the first readable segment
 * that holds the whole range. Returns 1 when one did. */
int MbCache_Read(DlPlayHeldProgram *cache, u32 address, void *dst, u32 size)
{
    int found = 0;
    OSIntrMode enabled = OS_DisableInterrupts();
    DlPlayHeldBlock *segment = cache->pages;
    DlPlayHeldBlock *end = cache->pages + DLPLAY_HELD_BLOCK_SLOTS;

    for (; segment < end; segment++) {
        int offset;

        if (segment->state < DLPLAY_HELD_BLOCK_USABLE) {
            continue;
        }
        offset = address - segment->address;
        if (offset < 0 || offset + size > segment->size) {
            continue;
        }
        Mem_CopyBytes(segment->buffer + offset, dst, size);
        found = 1;
        cache->readSinceLoad = found;
        break;
    }
    OS_RestoreInterrupts(enabled);
    return found;
}
