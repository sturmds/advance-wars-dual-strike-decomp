#include "sdk/snd.h"
#include "sdk/os.h"

/* Handles a reply word from the ARM7 driver (guessed). */
void func_021107a0(u32 data);

void SoundAlarm_OnArm7Message(int tag, u32 data, int err)
{
    int irq = OS_DisableInterrupts();

    func_021107a0(data);
    OS_RestoreInterrupts(irq);
}
