#include "types.h"
#include "sdk/card_arm7.h"

extern OSThreadInfo data_arm7_0380790c;
extern CARDiRomPage data_arm7_03809880;

void func_arm7_037fdd34(u32 dmaNo);
void CardRom_ReadPagesByCpu_Arm7(CARDiRomPage *page);

/* Starts a ROM read of len bytes from src to dst. dmaNo 0 to 3 is a DMA channel to stop first. */
void CardRom_BeginCpuRead(u32 dmaNo, u32 src, u8 *dst, u32 len)
{
    CARDiRomPage *page = &data_arm7_03809880;

    data_arm7_03809280.dmaNo = dmaNo;
    data_arm7_03809280.src = src;
    data_arm7_03809280.dst = dst;
    data_arm7_03809280.len = len;
    data_arm7_03809280.callback = NULL;
    data_arm7_03809280.callbackArg = NULL;
    if (dmaNo <= 3) {
        func_arm7_037fdd34(dmaNo);
    }
    page->control = (BOOT_HEADER_CARD_TIMING & ~CARD_COMMAND_MASK) | (CARD_START | CARD_RESET_HI | GAMECARD_XFER_ONE_PAGE);
    data_arm7_03809280.waiter = data_arm7_0380790c.current;
    CardRom_ReadPagesByCpu_Arm7(page);
}
