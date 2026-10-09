#include "types.h"
#include "game/sound_handle.h"

void Sound_StopHandle(SoundHandle *handle, int fadeFrames);

extern SoundHandle data_0218feb8;           /* second sound effect handle */
extern BOOL data_0218fec0;                  /* sound system ready */

/* Stops the sound started by Sound_PlaySubSe, fading out over
 * fadeFrames frames. */
void Sound_StopSubSe(int fadeFrames)
{
    if (data_0218fec0 == 0) {
        return;
    }
    Sound_StopHandle(&data_0218feb8, fadeFrames);
}
