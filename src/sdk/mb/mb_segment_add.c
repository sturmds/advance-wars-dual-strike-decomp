#include "types.h"
#include "sdk/mb.h"
#include "sdk/os.h"

/* Stores a segment in the first empty slot. Halts when all slots are in use. */
void MbSegment_Add(DlPlayProgramChunkSet *list, u32 address, u32 size, u8 *data, u32 state)
{
    OSIntrMode enabled = OS_DisableInterrupts();
    DlPlayProgramChunk *segment = list->segments;
    DlPlayProgramChunk *end = list->segments + DLPLAY_KEPT_CHUNK_SLOTS;

    for (;; segment++) {
        if (segment >= end) {
            OS_Terminate();
        }
        if (segment->state == 0) {
            segment->address = address;
            segment->size = size;
            segment->data = data;
            segment->state = state;
            break;
        }
    }
    OS_RestoreInterrupts(enabled);
}
