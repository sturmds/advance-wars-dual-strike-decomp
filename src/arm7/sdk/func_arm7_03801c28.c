#include "types.h"
#include "sdk/snd.h"

void func_arm7_037fee6c(void);
void func_arm7_037fee70(void);

void *func_arm7_03801c28(SoundWaveArc *arc, u32 waveNo)
{
    u32 entry;
    u8 *wave;

    func_arm7_037fee70();
    entry = arc->table[waveNo];
    if (entry != 0) {
        wave = (u8 *)entry;
        if (entry < 0x2000000) {
            wave = (u8 *)arc + entry;
        }
    } else {
        wave = NULL;
    }
    func_arm7_037fee6c();
    return wave;
}
