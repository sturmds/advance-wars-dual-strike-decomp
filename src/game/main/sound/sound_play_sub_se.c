#include "types.h"
#include "game/sound_handle.h"
#include "game/sound.h"

void Sound_PlayOnHandle(SoundHandle* handle, u32 soundId);

/* Plays soundId on the second sound effect handle, which
 * Sound_StopSubSe can stop. */
void Sound_PlaySubSe(u32 soundId) {
  if (gSoundReady == 0) {
    return;
  }
  Sound_PlayOnHandle(&gSubSeHandle, soundId);
}
