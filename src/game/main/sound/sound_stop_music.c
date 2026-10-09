#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndPlayerStopSeq */
void func_0210589c(SoundHandle *handle, int fadeFrames);

extern SoundHandle gMusicHandle;
extern BOOL gSoundReady;

/* Stops the music with a one-frame fade. */
void Sound_StopMusic(void)
{
    if (gSoundReady == 0) {
        return;
    }
    func_0210589c(&gMusicHandle, 1);
}
