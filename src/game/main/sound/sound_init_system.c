#include "types.h"
#include "game/sound.h"

void SoundSystem_Init(void);
void NNS_SndHandleInit(SoundHandle* handle);
void NNS_SndStrmHandleInit(void** handle);

/* Boot-time sound setup: starts the NitroSystem sound library, clears the
 * music, sound effect and stream handles and marks the Sound_ calls as not
 * ready until Sound_Init has run. */
void Sound_InitSystem(void) {
  SoundSystem_Init();
  NNS_SndHandleInit(&gMusicHandle);
  NNS_SndHandleInit(&gSeHandle);
  NNS_SndStrmHandleInit(&gStrmHandle);
  gSoundReady = FALSE;
}
