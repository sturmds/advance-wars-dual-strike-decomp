#include "types.h"
#include "sdk/snd.h"
#include "sdk/os.h"

void func_0210fec0(void);
void func_0210fed4(void);

/* Records where wave waveNo is loaded (NULL to forget it) and flushes the entry
 * from the data cache for the sound driver. */
void SND_SetWaveDataAddress(SoundWaveArc *arc, u32 waveNo, void *wave)
{
    func_0210fed4();
    arc->table[waveNo] = (u32)wave;
    DC_StoreRange(&arc->table[waveNo], 4);
    func_0210fec0();
}
