#include "types.h"
#include "sdk/pm.h"
#include "sdk/tp.h"

void NVRAM_AnalyzeCommand(u32 data);   /* NVRAM request word */
void PM_AnalyzeCommand(u32 data);   /* power chip request word */
void func_arm7_03805da8(u32 data);   /* microphone request word */

/* PXI receive callback of the four SPI devices: hands each word to its
 * device's request parser. Words that arrive with the error bit are ignored. */
void SerialDev_OnFifoMessage(u32 tag, u32 data, int err)
{
    if (err) {
        return;
    }

    switch (tag) {
    case FIFO_TAG_TOUCH:
        TP_AnalyzeCommand(data);
        break;
    case PXI_FIFO_TAG_MIC:
        func_arm7_03805da8(data);
        break;
    case PXI_FIFO_TAG_PM:
        PM_AnalyzeCommand(data);
        break;
    case PXI_FIFO_TAG_NVRAM:
        NVRAM_AnalyzeCommand(data);
        break;
    }
}
