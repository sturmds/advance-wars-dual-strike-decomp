#ifndef GAME_SOUND_H
#define GAME_SOUND_H

#include "types.h"
#include "game/sound_handle.h"

/* Sound state the Sound_* wrappers share. They do nothing while
 * gSoundReady is FALSE. */
extern BOOL gSoundReady;
extern void* gSoundHeap; /* NitroSystem sound heap */
extern SoundHandle gMusicHandle;
extern SoundHandle gSeHandle;
extern SoundHandle gSubSeHandle; /* second sound effect */
extern void* gStrmHandle;        /* NitroSystem stream handle */
/* sound channels locked so far, one bit each */
extern u32 gLockedSoundChannels;
extern u32 data_022a8044;
extern u32 data_022a8048; /* bit n set: alarm n is in use */

/* NitroSystem sound library, named after the NitroSystem API. */
void NNS_SndMain(void);
int SoundHeap_NewestSaveLevel(void* heap);
int NNS_SndHeapSaveState(void* heap);
void NNS_SndHeapLoadState(void* heap, int level);
void NNS_SndHeapDestroy(void* heap);
void NNS_SndPlayerMoveVolume(SoundHandle* handle, int targetVolume, int frames);
void NNS_SndPlayerSetVolume(SoundHandle* handle, int volume);
void NNS_SndPlayerStopSeq(SoundHandle* handle, int fadeFrames);
void NNS_SndPlayerStopSeqAll(int fadeFrames);
BOOL NNS_SndArcPlayerStartSeq(SoundHandle* handle, int seqNo);
BOOL NNS_SndArcPlayerStartSeqArc(SoundHandle* handle, int seqArcNo, int index);
BOOL NNS_SndArcStrmStart(void** handle, int strmNo, u32 offset);
void NNS_SndArcStrmStop(void** handle, int fadeFrames);

#endif
