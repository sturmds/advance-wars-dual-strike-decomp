#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndPlayerMoveVolume */
void func_021057cc(SoundHandle *handle, int targetVolume, int frames);

extern SoundHandle gSeHandle;
extern BOOL gSoundReady;

/* Fades the sound effect volume from startVolume to targetVolume over
 * frames frames. startVolume -1 starts from the player's set volume. */
void Sound_FadeSeVolume(int startVolume, int targetVolume, int frames)
{
    if (gSoundReady == 0) {
        return;
    }
    if (gSeHandle.player == NULL) {
        return;
    }
    if (startVolume == -1) {
        startVolume = gSeHandle.player->volume;
    }
    func_021057cc(&gSeHandle, startVolume, 0);
    func_021057cc(&gSeHandle, targetVolume, frames);
}
