#include "sdk/snd.h"
#include "sdk/os.h"

void SoundAlarm_OnArm7Message(int tag, u32 data, int err)
{
    int irq = OS_DisableInterrupts();

    SoundAlarm_RunIfTagMatches(data);
    OS_RestoreInterrupts(irq);
}
