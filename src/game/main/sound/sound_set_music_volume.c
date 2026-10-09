#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndPlayerSetVolume */
void func_02105824(SoundHandle *handle, int volume);

extern SoundHandle data_0218fea8;           /* music handle */
extern BOOL data_0218fec0;                  /* sound system ready */

void Sound_SetMusicVolume(int volume)
{
    if (data_0218fec0 == 0) {
        return;
    }
    func_02105824(&data_0218fea8, volume);
}
