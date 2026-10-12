#include "sdk/snd.h"
#include "sdk/os_arm7.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void SND_SendWakeupMessage(void);

#define MAIN_RAM_START 0x02000000

/* PXI receive callback of the sound tag: a command list address goes to the driver queue, 0 wakes the driver. */
void SndArm7_OnPxiReceive(int tag, u32 data)
{
    int irqState = func_arm7_037fdb80();

    if (data >= MAIN_RAM_START) {
        MsgQueue_PushBack_Arm7(&data_arm7_03809210, (OSMessage)data, 0);
    } else if (data == 0) {
        SND_SendWakeupMessage();
    }
    func_arm7_037fdb94(irqState);
}
