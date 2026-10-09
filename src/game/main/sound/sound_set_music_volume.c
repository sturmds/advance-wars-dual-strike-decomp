#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndPlayerSetVolume */
void func_02105824(SoundHandle *handle, int volume);

extern SoundHandle gMusicHandle;
extern BOOL gSoundReady;

void Sound_SetMusicVolume(int volume)
{
    if (gSoundReady == 0) {
        return;
    }
    func_02105824(&gMusicHandle, volume);
}
