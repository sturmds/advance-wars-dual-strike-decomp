#include "sdk/card.h"
#include "sdk/ctrdg.h"

int PXI_SendWordByFifo(int tag, u32 word, int err);

/* Sends a word under the card removal tag, waiting between tries until the FIFO takes it. */
void Card_PushRemovalTagWord(u32 word, u32 waitCount)
{
    if (PXI_SendWordByFifo(FIFO_TAG_CARD_REMOVAL, word, 0) == 0) {
        return;
    }
    do {
        SVC_WaitByLoop(waitCount);
    } while (PXI_SendWordByFifo(FIFO_TAG_CARD_REMOVAL, word, 0) != 0);
}
