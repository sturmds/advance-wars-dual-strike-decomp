#include "types.h"
#include "game/sound_handle.h"

void Sound_PlayOnHandle(SoundHandle *handle, u32 soundId);

extern SoundHandle data_0218feb8;           /* second sound effect handle */
extern BOOL data_0218fec0;                  /* sound system ready */

/* Plays soundId on the second sound effect handle, which
 * Sound_StopSubSe can stop. */
void Sound_PlaySubSe(u32 soundId)
{
    if (data_0218fec0 == 0) {
        return;
    }
    Sound_PlayOnHandle(&data_0218feb8, soundId);
}
