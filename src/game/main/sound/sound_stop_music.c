#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndPlayerStopSeq */
void func_0210589c(SoundHandle *handle, int fadeFrames);

extern SoundHandle data_0218fea8;           /* music handle */
extern BOOL data_0218fec0;                  /* sound system ready */

/* Stops the music with a one-frame fade. */
void Sound_StopMusic(void)
{
    if (data_0218fec0 == 0) {
        return;
    }
    func_0210589c(&data_0218fea8, 1);
}
