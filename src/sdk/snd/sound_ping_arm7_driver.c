#include "sdk/snd.h"

/* Sends one word on the PXI FIFO; negative when the FIFO is full. */
int func_02110f88(int tag, u32 data, int err);

void Sound_PingArm7Driver(void)
{
    while (func_02110f88(SOUND_FIFO_TAG, 0, 0) < 0) {
    }
}
