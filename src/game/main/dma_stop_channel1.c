#include "types.h"

void func_0210efb0(int channel);   /* SDK: stops a DMA channel */

extern u8 data_0216e054[];   /* [channel]: nonzero while the game tracks that DMA channel */

/* Stops DMA channel 1 and clears its flag. */
void Dma_StopChannel1(void)
{
    func_0210efb0(1);
    data_0216e054[1] = 0;
}
