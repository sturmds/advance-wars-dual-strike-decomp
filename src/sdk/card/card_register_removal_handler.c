#include "sdk/card.h"
#include "sdk/pxi.h"

void Card_OnRemovalMessage(u32 tag, u32 data, int err);

/* Registers the card removal receive callback. */
void Card_RegisterRemovalHandler(void) {
  PXI_Init();
  PXI_SetFifoRecvCallback(FIFO_TAG_CARD_REMOVAL, Card_OnRemovalMessage);
  data_022ac324 = NULL;
}
