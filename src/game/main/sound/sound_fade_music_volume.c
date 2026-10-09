#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndPlayerMoveVolume */
void func_021057cc(SoundHandle *handle, int targetVolume, int frames);

extern SoundHandle gMusicHandle;
extern BOOL gSoundReady;

/* Fades the music volume from startVolume to targetVolume over frames
 * frames. startVolume -1 starts from the player's set volume. */
void Sound_FadeMusicVolume(int startVolume, int targetVolume, int frames)
{
    if (gSoundReady == 0) {
        return;
    }
    if (gMusicHandle.player == NULL) {
        return;
    }
    if (startVolume == -1) {
        startVolume = gMusicHandle.player->volume;
    }
    func_021057cc(&gMusicHandle, startVolume, 0);
    func_021057cc(&gMusicHandle, targetVolume, frames);
}
