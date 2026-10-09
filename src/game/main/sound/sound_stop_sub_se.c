#include "types.h"
#include "game/sound_handle.h"

void Sound_StopHandle(SoundHandle *handle, int fadeFrames);

extern SoundHandle gSubSeHandle;
extern BOOL gSoundReady;

/* Stops the sound started by Sound_PlaySubSe, fading out over
 * fadeFrames frames. */
void Sound_StopSubSe(int fadeFrames)
{
    if (gSoundReady == 0) {
        return;
    }
    Sound_StopHandle(&gSubSeHandle, fadeFrames);
}
