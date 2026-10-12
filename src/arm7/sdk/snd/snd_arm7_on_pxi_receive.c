#include "sdk/snd.h"

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);
void func_arm7_037fc44c(OSMessageQueue *queue, OSMessage message, int flags);
void func_arm7_037fee74(void);

#define MAIN_RAM_START 0x02000000

/* PXI receive callback of the sound tag: a command list address goes to the driver queue, 0 wakes the driver. */
void SndArm7_OnPxiReceive(int tag, u32 data)
{
    int irqState = func_arm7_037fdb80();

    if (data >= MAIN_RAM_START) {
        func_arm7_037fc44c(&data_arm7_03809210, (OSMessage)data, 0);
    } else if (data == 0) {
        func_arm7_037fee74();
    }
    func_arm7_037fdb94(irqState);
}
