#include "types.h"
#include "game/sound_handle.h"

void Sound_PlayOnHandle(SoundHandle *handle, u32 soundId);

extern SoundHandle gSeHandle;
extern BOOL gSoundReady;

/* Plays sound effect soundId (see Sound_PlayOnHandle for the id format). */
void Sound_PlaySe(u32 soundId)
{
    if (gSoundReady == 0) {
        return;
    }
    Sound_PlayOnHandle(&gSeHandle, soundId);
}
