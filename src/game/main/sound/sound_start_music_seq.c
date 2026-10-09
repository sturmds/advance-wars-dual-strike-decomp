#include "types.h"
#include "game/sound_handle.h"

/* NitroSystem NNS_SndArcPlayerStartSeq: returns TRUE if the sequence started. */
BOOL func_02107ab0(SoundHandle *handle, int seqNo);

extern SoundHandle gMusicHandle;
extern BOOL gSoundReady;

/* Starts sound archive sequence seqNo on the music handle. */
void Sound_StartMusicSeq(int seqNo)
{
    if (gSoundReady == 0) {
        return;
    }
    func_02107ab0(&gMusicHandle, seqNo);
}
