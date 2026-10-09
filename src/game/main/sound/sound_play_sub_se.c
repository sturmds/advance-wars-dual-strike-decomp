#include "types.h"
#include "game/sound_handle.h"

void Sound_PlayOnHandle(SoundHandle *handle, u32 soundId);

extern SoundHandle gSubSeHandle;
extern BOOL gSoundReady;

/* Plays soundId on the second sound effect handle, which
 * Sound_StopSubSe can stop. */
void Sound_PlaySubSe(u32 soundId)
{
    if (gSoundReady == 0) {
        return;
    }
    Sound_PlayOnHandle(&gSubSeHandle, soundId);
}
