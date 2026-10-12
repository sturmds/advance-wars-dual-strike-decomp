#include "types.h"
#include "sdk/snd.h"

void *func_arm7_03801c28(SoundWaveArc *arc, u32 waveNo)
{
    u32 entry;
    u8 *wave;

    SNDi_LockMutex();
    entry = arc->table[waveNo];
    if (entry != 0) {
        wave = (u8 *)entry;
        if (entry < 0x2000000) {
            wave = (u8 *)arc + entry;
        }
    } else {
        wave = NULL;
    }
    SNDi_UnlockMutex();
    return wave;
}
