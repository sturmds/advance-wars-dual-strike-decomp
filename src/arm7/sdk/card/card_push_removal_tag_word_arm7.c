#include "types.h"
#include "sdk/os_arm7.h"
#include "sdk/card_arm7.h"
#include "sdk/pxi.h"

/* Sends a word under the card removal tag, waiting between tries until the FIFO takes it. */
void Card_PushRemovalTagWord_Arm7(u32 word, u32 waitCount) {
  while (Fifo_PushWord_Arm7(FIFO_TAG_CARD_REMOVAL, word, 0) != 0) {
    Cpu_DelayLoop_Arm7(waitCount);
  }
}
