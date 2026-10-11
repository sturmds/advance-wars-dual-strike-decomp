#include "sdk/card.h"

void func_02110e58(void);
void PXI_SetFifoRecvCallback(int tag, void (*callback)(int, u32, int));
void Card_OnRemovalMessage(int tag, u32 data, int err);

/* Registers the card removal receive callback. */
void Card_RegisterRemovalHandler(void)
{
    func_02110e58();
    PXI_SetFifoRecvCallback(FIFO_TAG_CARD_REMOVAL, Card_OnRemovalMessage);
    data_022ac324 = NULL;
}
