#include "types.h"
#include "game/sound.h"
#include "game/sound_arc.h"
#include "game/sound_heap.h"

extern void *data_0218feb0;
extern u8 data_0218ff58[];
extern SoundArc gSoundArc;

SoundHeap *SoundHeap_Create(u32 mem, u32 size);
BOOL func_02106704(SoundArc *arc, void *heap, int symbolLoad);
void Sound_CreatePlayerHeaps(void *heap);
void func_0210902c(void *heap);
void Sound_SaveHeapState(void);

/* Like Sound_Init, but gSoundArc is filled by func_02106704 without
 * opening the archive file (name guessed from that difference). */
void Sound_InitFromMemory(void)
{
    if (gSoundReady != 0) {
        return;
    }
    data_0218feb0 = data_0218ff58;
    gSoundHeap = SoundHeap_Create((u32)data_0218ff58, 0xf8000);
    func_02106704(&gSoundArc, gSoundHeap, 0);
    Sound_CreatePlayerHeaps(gSoundHeap);
    func_0210902c(gSoundHeap);
    gSoundReady = TRUE;
    Sound_SaveHeapState();
}
