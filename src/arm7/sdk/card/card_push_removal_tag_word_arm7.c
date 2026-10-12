#include "types.h"
#include "sdk/card_arm7.h"
#include "sdk/pxi.h"

void func_arm7_037fdc04(int count); /* SVC_WaitByLoop through a veneer */

/* Sends a word under the card removal tag, waiting between tries until the FIFO takes it. */
void Card_PushRemovalTagWord_Arm7(u32 word, u32 waitCount) {
  while (Fifo_PushWord_Arm7(FIFO_TAG_CARD_REMOVAL, word, 0) != 0) {
    func_arm7_037fdc04(waitCount);
  }
}
