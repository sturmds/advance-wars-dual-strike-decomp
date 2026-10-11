#include "sdk/card.h"
#include "sdk/ctrdg.h"

int func_02110f88(int tag, u32 word, int err);

/* Sends a word under the card removal tag, waiting between tries until the FIFO takes it. */
void Card_PushRemovalTagWord(u32 word, u32 waitCount)
{
    if (func_02110f88(FIFO_TAG_CARD_REMOVAL, word, 0) == 0) {
        return;
    }
    do {
        WaitByLoop(waitCount);
    } while (func_02110f88(FIFO_TAG_CARD_REMOVAL, word, 0) != 0);
}
