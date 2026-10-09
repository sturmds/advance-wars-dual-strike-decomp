#include "types.h"
#include "game/sound.h"

void *func_02110554(int block);
void func_02105544(void);
void SoundOutput_Update(void);
void SoundStream_UpdateAll(void);
void func_021102b0(int block);

/* Per-frame NitroSystem sound update. Reads replies from the sound
 * processor until none are left, then runs the update steps. */
void NNS_SndMain(void)
{
    while (func_02110554(0) != NULL) {
    }
    func_02105544();
    SoundOutput_Update();
    SoundStream_UpdateAll();
    func_021102b0(0);
}
