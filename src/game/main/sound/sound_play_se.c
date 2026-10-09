#include "types.h"
#include "game/sound_handle.h"

void Sound_PlayOnHandle(SoundHandle *handle, u32 soundId);

extern SoundHandle data_0218feb4;           /* sound effect handle */
extern BOOL data_0218fec0;                  /* sound system ready */

/* Plays sound effect soundId (see Sound_PlayOnHandle for the id format). */
void Sound_PlaySe(u32 soundId)
{
    if (data_0218fec0 == 0) {
        return;
    }
    Sound_PlayOnHandle(&data_0218feb4, soundId);
}
