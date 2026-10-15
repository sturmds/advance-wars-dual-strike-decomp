#include "types.h"
#include "game/sound.h"
#include "game/sound_arc.h"
#include "game/sound_heap.h"

extern void* data_0218feb0;
extern SoundArc gSoundArc;
static char gSoundArcPath[] = {
#include "data/main/gSoundArcPath.inc.c"
};

SoundHeap* SoundHeap_Create(u32 mem, u32 size);
void SoundArc_Open(SoundArc* arc, const char* path, void* heap, int symbolLoad);
void Sound_CreatePlayerHeaps(void* heap);
void SoundStream_Init(u32 threadPriority, void* heap);
void Sound_SaveHeapState(void);

/* Sets up the sound system once: a 0xf8000-byte sound heap, the sound
 * archive file, the sequence players and the stream. Later calls do nothing. */
void Sound_Init(void) {
  if (gSoundReady != 0) {
    return;
  }
  data_0218feb0 = data_0218ff58;
  gSoundHeap = SoundHeap_Create((u32)data_0218ff58, 0xf8000);
  SoundArc_Open(&gSoundArc, gSoundArcPath, gSoundHeap, 0);
  Sound_CreatePlayerHeaps(gSoundHeap);
  SoundStream_Init(10, gSoundHeap);
  gSoundReady = TRUE;
  Sound_SaveHeapState();
}
