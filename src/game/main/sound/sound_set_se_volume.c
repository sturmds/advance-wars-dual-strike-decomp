#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndPlayerSetVolume */
void func_02105824(SoundHandle *handle, int volume);

extern SoundHandle data_0218feb4;           /* sound effect handle */
extern BOOL data_0218fec0;                  /* sound system ready */

void Sound_SetSeVolume(int volume)
{
    if (data_0218fec0 == 0) {
        return;
    }
    func_02105824(&data_0218feb4, volume);
}
