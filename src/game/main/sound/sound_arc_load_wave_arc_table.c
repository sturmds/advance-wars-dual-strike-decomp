#include "types.h"
#include "game/sound_arc.h"
#include "game/sound_wave_arc.h"

void *func_02106304(int fileId);
SoundArc *func_021066dc(void);
void func_021062e8(int fileId, void *loaded);
s32 SoundArc_ReadFile(u32 fileNo, void *dest, u32 size, u32 offset);
void *SoundHeap_AllocItem(void *heap, u32 size, void (*onUnload)(void *payload, u32 size, SoundArc *arc, int fileId),
                          SoundArc *arc, int fileId);
void SoundWaveArc_OnUnload(void *payload, u32 size, SoundArc *arc, int fileId);
void Mem_CopyBytes(const void *src, void *dest, u32 size);
void func_0210f52c(void *dest, int value, u32 size);
void func_0210e11c(void *start, u32 size);

extern SoundWaveArc data_022a8a64;         /* header of the wave archive being loaded */

/* Loads wave archive file fileId for loading its waves one at a time: only
 * the header and the wave table go into heap, the wave table cleared and its
 * file offsets copied after it (see SoundWaveArc). Returns the archive at once
 * when it is already in memory. With cache, records the address for the
 * current archive. Returns NULL when a read fails, there is no heap or the
 * heap is full. */
void *SoundArc_LoadWaveArcTable(int fileId, void *heap, int cache)
{
    SoundWaveArc *waveArc = func_02106304(fileId);
    u32 allocSize;
    u32 tableSize;
    s32 readSize;

    if (waveArc == NULL) {
        readSize = SoundArc_ReadFile(fileId, &data_022a8a64, SOUND_WAVE_ARC_HEADER_SIZE, 0);
        if (readSize != SOUND_WAVE_ARC_HEADER_SIZE) {
            return NULL;
        }
        tableSize = data_022a8a64.count * sizeof(u32);
        allocSize = SOUND_WAVE_ARC_HEADER_SIZE + tableSize * 2;
        if (heap == NULL) {
            return NULL;
        }
        waveArc = SoundHeap_AllocItem(heap, allocSize + 32, SoundWaveArc_OnUnload, cache ? func_021066dc() : NULL, fileId);
        if (waveArc == NULL) {
            return NULL;
        }
        readSize = SoundArc_ReadFile(fileId, waveArc, SOUND_WAVE_ARC_HEADER_SIZE + tableSize, 0);
        if (readSize != SOUND_WAVE_ARC_HEADER_SIZE + tableSize) {
            return NULL;
        }
        Mem_CopyBytes(waveArc->table, &waveArc->table[waveArc->count], tableSize);
        func_0210f52c(waveArc->table, 0, tableSize);
        func_0210e11c(waveArc, allocSize);
        if (cache) {
            func_021062e8(fileId, waveArc);
        }
    }
    return waveArc;
}
