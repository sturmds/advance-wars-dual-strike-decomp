#include "types.h"
#include "game/sound_arc.h"

/* The heap calls onUnload with the item, its size and arg3. */
void *SoundHeap_AllocItem(void *heap, u32 size, void (*onUnload)(void *payload, u32 size, SoundArc *arc),
                          SoundArc *arc, u32 arg4);
void SoundArc_ForgetInfo(void *payload, u32 size, SoundArc *arc);
void SoundArc_ForgetFileTable(void *payload, u32 size, SoundArc *arc);
void SoundArc_ForgetSymbols(void *payload, u32 size, SoundArc *arc);

/* Reads the archive's header, then, when heap is given, loads the INFO block
 * and the file table into it, and the symbol block too when symbolLoad is set
 * and the archive has one. Each block clears its pointer in arc when the heap
 * frees it. Returns FALSE when a seek or a read fails or the heap is full. */
BOOL SoundArc_ReadTables(SoundArc *arc, void *heap, int symbolLoad)
{
    s32 readSize;

    if (!FS_SeekFile(&arc->file, 0, 0)) {
        return FALSE;
    }
    if (FS_ReadFile(&arc->file, arc, SOUND_ARC_HEADER_SIZE) != SOUND_ARC_HEADER_SIZE) {
        return FALSE;
    }
    if (heap != NULL) {
        arc->info = SoundHeap_AllocItem(heap, arc->infoSize, SoundArc_ForgetInfo, arc, 0);
        if (arc->info == NULL) {
            return FALSE;
        }
        if (!FS_SeekFile(&arc->file, arc->infoOffset, 0)) {
            return FALSE;
        }
        readSize = FS_ReadFile(&arc->file, arc->info, arc->infoSize);
        if (readSize != arc->infoSize) {
            return FALSE;
        }

        arc->files = SoundHeap_AllocItem(heap, arc->fatSize, SoundArc_ForgetFileTable, arc, 0);
        if (arc->files == NULL) {
            return FALSE;
        }
        if (!FS_SeekFile(&arc->file, arc->fatOffset, 0)) {
            return FALSE;
        }
        readSize = FS_ReadFile(&arc->file, arc->files, arc->fatSize);
        if (readSize != arc->fatSize) {
            return FALSE;
        }

        if (symbolLoad && arc->symbolSize != 0) {
            arc->unk_84 = (u32)SoundHeap_AllocItem(heap, arc->symbolSize, SoundArc_ForgetSymbols, arc, 0);
            if (arc->unk_84 == 0) {
                return FALSE;
            }
            if (!FS_SeekFile(&arc->file, arc->symbolOffset, 0)) {
                return FALSE;
            }
            readSize = FS_ReadFile(&arc->file, (void *)arc->unk_84, arc->symbolSize);
            if (readSize != arc->symbolSize) {
                return FALSE;
            }
        }
    }
    return TRUE;
}
