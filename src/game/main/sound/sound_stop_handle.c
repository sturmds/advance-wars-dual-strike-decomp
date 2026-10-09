#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndPlayerStopSeq */
void func_0210589c(SoundHandle *handle, int fadeFrames);

extern BOOL gSoundReady;

/* Stops the sequence on handle, fading out over fadeFrames frames. */
void Sound_StopHandle(SoundHandle *handle, int fadeFrames)
{
    if (gSoundReady == 0) {
        return;
    }
    func_0210589c(handle, fadeFrames);
}
