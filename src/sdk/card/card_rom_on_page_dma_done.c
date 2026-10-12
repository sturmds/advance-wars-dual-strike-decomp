#include "sdk/card.h"

int MI_StopDma(int dmaNo);
void CardRom_StartPageDma(void);
void CARDi_EndTask(void);

/* Card data interrupt: one page arrived by DMA; reads the next or ends the request. */
void CardRom_OnPageDmaDone(void) {
  CARDiCommon* common;

  MI_StopDma(data_022abb00.unk_24);
  common = &data_022abb00;
  common->src += GAMECARD_ROM_PAGE_BYTES;
  common->dst += GAMECARD_ROM_PAGE_BYTES;
  common->len -= GAMECARD_ROM_PAGE_BYTES;
  if (common->len == 0) {
    OS_DisableIrqMask(IRQ_CARD_DATA);
    OS_ResetRequestIrqMask(IRQ_CARD_DATA);
    CARDi_EndTask();
  } else {
    CardRom_StartPageDma();
  }
}
