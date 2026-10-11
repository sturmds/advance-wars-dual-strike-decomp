#include "sdk/snd.h"
#include "sdk/os.h"

/* Handles a reply word from the ARM7 driver (guessed). */
void SoundAlarm_RunIfTagMatches(u32 data);

void SoundAlarm_OnArm7Message(int tag, u32 data, int err)
{
    int irq = OS_DisableInterrupts();

    SoundAlarm_RunIfTagMatches(data);
    OS_RestoreInterrupts(irq);
}
