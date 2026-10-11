#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

void Mem_CopyBytes(const void *src, void *dst, u32 size);

/* Copies size bytes at program address to dst from the first readable segment
 * that holds the whole range. Returns 1 when one did. */
int MbSegment_Read(DlPlayProgramChunkSet *list, u32 address, void *dst, u32 size)
{
    int found = 0;
    OSIntrMode enabled = OS_DisableInterrupts();
    DlPlayProgramChunk *segment = list->segments;
    DlPlayProgramChunk *end = list->segments + DLPLAY_KEPT_CHUNK_SLOTS;

    for (; segment < end; segment++) {
        int offset;

        if (segment->state < 2) {
            continue;
        }
        offset = address - segment->address;
        if (offset < 0 || offset + size > segment->size) {
            continue;
        }
        Mem_CopyBytes(segment->data + offset, dst, size);
        found = 1;
        list->unk_00 = found;
        break;
    }
    OS_RestoreInterrupts(enabled);
    return found;
}
