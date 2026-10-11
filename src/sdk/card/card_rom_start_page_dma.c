/* needed to match: the original was built without copy propagation */
#pragma opt_propagation off
#include "sdk/card.h"

void MIi_CardDmaCopy32(int dmaNo, volatile u32 *src, void *dst, u32 size);
void CardRom_LoadCommandBytes(u32 high, u32 low);

/* Starts the DMA for one page and sends the read command. */
void CardRom_StartPageDma(void)
{
    CARDiCommon *common = &data_022abb00;

    MIi_CardDmaCopy32(common->unk_24, &IO_GAMECARD_READ_PORT, (void *)common->dst, GAMECARD_ROM_PAGE_BYTES);
    CardRom_LoadCommandBytes((CARD_CMD_READ_PAGE << 24) | (common->src >> 8), common->src << 24);
    IO_GAMECARD_ROM_CTRL = data_022ac100.control;
}
