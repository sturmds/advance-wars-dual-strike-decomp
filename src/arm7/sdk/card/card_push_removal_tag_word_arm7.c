#include "types.h"
#include "sdk/card.h"

int func_arm7_037fe1cc(u32 tag, u32 data, int err);   /* PXI_SendWordByFifo: 0 once queued */
void func_arm7_037fdc04(int count);                   /* SVC_WaitByLoop through a veneer */

/* Sends a word under the card removal tag, waiting between tries until the FIFO takes it. */
void Card_PushRemovalTagWord_Arm7(u32 word, u32 waitCount)
{
    while (func_arm7_037fe1cc(FIFO_TAG_CARD_REMOVAL, word, 0) != 0) {
        func_arm7_037fdc04(waitCount);
    }
}
