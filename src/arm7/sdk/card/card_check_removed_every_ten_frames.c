#include "types.h"
#include "sdk/card_arm7.h"
#include "sdk/pm.h"

void Card_PushRemovalTagWord_Arm7(u32 word, u32 waitCount);

extern u32 gCardPulledOut; /* set once the card was found pulled out */
/* VBlank count of the next check; 0xffffffff before the first call */
static u32 gCardNextPollFrame = {
#include "data/arm7/gCardNextPollFrame.inc.c"
};
/* 1 until the first check ran */
static u32 gCardFirstPoll = {
#include "data/arm7/gCardFirstPoll.inc.c"
};

/*
 * Checks the game card every 10 frames, except on a multiboot download. Once
 * the card is found pulled out, tells the ARM9 and stops checking. If the card
 * was already gone at the first check, it stops without telling the ARM9.
 */
void Card_CheckRemovedEveryTenFrames(void) {
  if (gCardPulledOut != 0) {
    return;
  }
  if (*BOOT_ORIGIN_WORD == BOOT_ORIGIN_WIRELESS) {
    return;
  }
  if (gCardNextPollFrame == 0xffffffff) {
    gCardNextPollFrame = *SHARED_FRAME_COUNTER + 10;
    return;
  }
  if (*SHARED_FRAME_COUNTER < gCardNextPollFrame) {
    return;
  }
  gCardNextPollFrame = *SHARED_FRAME_COUNTER + 10;
  if (CARD_IsPulledOut()) {
    gCardPulledOut = 1;
    if (gCardFirstPoll != 0) {
      return;
    }
  }
  gCardFirstPoll = 0;
  if (gCardPulledOut != 0) {
    Card_PushRemovalTagWord_Arm7(GAMECARD_MSG_REMOVED, 100);
  }
}
