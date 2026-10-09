#include "types.h"
#include "game/sound_handle.h"

void Sound_PlaySe(u32 soundId);
/* NitroSystem NNS_SndPlayerSetVolume */
void func_02105824(SoundHandle *handle, int volume);

extern SoundHandle gSeHandle;
extern BOOL gSoundReady;

/* Plays sound effect soundId at the given player volume. */
void Sound_PlaySeAtVolume(u32 soundId, int volume)
{
    if (gSoundReady == 0) {
        return;
    }
    Sound_PlaySe(soundId);
    func_02105824(&gSeHandle, volume);
}
