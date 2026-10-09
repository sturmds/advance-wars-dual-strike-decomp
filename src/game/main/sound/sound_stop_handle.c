#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndPlayerStopSeq */
void func_0210589c(SoundHandle *handle, int fadeFrames);

extern BOOL data_0218fec0;                  /* sound system ready */

/* Stops the sequence on handle, fading out over fadeFrames frames. */
void Sound_StopHandle(SoundHandle *handle, int fadeFrames)
{
    if (data_0218fec0 == 0) {
        return;
    }
    func_0210589c(handle, fadeFrames);
}
