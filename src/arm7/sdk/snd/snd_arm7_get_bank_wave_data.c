#include "sdk/snd.h"

void *func_arm7_03801c28(SoundWaveArc *arc, u32 waveNo);

/* Returns wave waveNo of the bank's wave archive in slot waveArcSlot, or NULL
 * when no archive is linked there, the number is out of range or the wave is not loaded. */
void *SndArm7_GetBankWaveData(SoundBank *bank, int waveArcSlot, u32 waveNo)
{
    SoundWaveArc *arc = bank->waveArcs[waveArcSlot].waveArc;

    if (arc == NULL) {
        return NULL;
    }
    if (waveNo >= arc->count) {
        return NULL;
    }
    return func_arm7_03801c28(arc, waveNo);
}
