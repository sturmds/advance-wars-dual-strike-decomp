#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndPlayerMoveVolume */
void func_021057cc(SoundHandle *handle, int targetVolume, int frames);

extern SoundHandle data_0218feb4;           /* sound effect handle */
extern BOOL data_0218fec0;                  /* sound system ready */

/* Fades the sound effect volume from startVolume to targetVolume over
 * frames frames. startVolume -1 starts from the player's set volume. */
void Sound_FadeSeVolume(int startVolume, int targetVolume, int frames)
{
    if (data_0218fec0 == 0) {
        return;
    }
    if (data_0218feb4.player == NULL) {
        return;
    }
    if (startVolume == -1) {
        startVolume = data_0218feb4.player->volume;
    }
    func_021057cc(&data_0218feb4, startVolume, 0);
    func_021057cc(&data_0218feb4, targetVolume, frames);
}
