#include "types.h"
#include "game/sound.h"

void *func_02110554(int block);
void SoundSeqPlayer_UpdateAll(void);
void SoundOutput_Update(void);
void SoundStream_UpdateAll(void);
void SND_FlushCommand(int block);

/* Per-frame NitroSystem sound update. Reads replies from the sound
 * processor until none are left, then runs the update steps. */
void NNS_SndMain(void)
{
    while (func_02110554(0) != NULL) {
    }
    SoundSeqPlayer_UpdateAll();
    SoundOutput_Update();
    SoundStream_UpdateAll();
    SND_FlushCommand(0);
}
