#include "sdk/os.h"

void func_0210cdf4(int dma, void (*handler)(void *), void *arg)
{
    int bit = dma + 8;

    gOsIrqSlots[dma].handler = handler;
    data_022a98c4[dma].arg = arg;
    data_022a98c0[dma].keepEnabled = OS_EnableIrqMask(1 << bit) & (1 << bit);
}
