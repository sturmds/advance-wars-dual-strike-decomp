#include "types.h"
#include "game/sound_handle.h"

void Sound_PlaySe(u32 soundId);
/* NitroSystem NNS_SndPlayerSetVolume */
void func_02105824(SoundHandle *handle, int volume);

extern SoundHandle data_0218feb4;           /* sound effect handle */
extern BOOL data_0218fec0;                  /* sound system ready */

/* Plays sound effect soundId at the given player volume. */
void Sound_PlaySeAtVolume(u32 soundId, int volume)
{
    if (data_0218fec0 == 0) {
        return;
    }
    Sound_PlaySe(soundId);
    func_02105824(&data_0218feb4, volume);
}
