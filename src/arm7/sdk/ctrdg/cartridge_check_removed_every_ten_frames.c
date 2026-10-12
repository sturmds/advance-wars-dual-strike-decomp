#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/pm.h"
#include "sdk/os_arm7.h"
#include "sdk/pxi.h"

extern u32 gCtrdgFirstPoll; /* 1 until the first check ran */
extern u32
    gCtrdgNextPollFrame; /* VBlank count of the next check; 0xffffffff before the first call */
extern u32
    gCtrdgPollStopped; /* set when no cartridge was there at the first check */
extern u32 gCtrdgPulledOut; /* set once the cartridge was found pulled out */

/*
 * Checks the GBA slot every 10 frames. Once the cartridge is found pulled
 * out, tells the ARM9 and stops checking. If there was no cartridge at the
 * first check, it stops without telling the ARM9.
 */
void Cartridge_CheckRemovedEveryTenFrames(void) {
  if (gCtrdgNextPollFrame == 0xffffffff) {
    gCtrdgNextPollFrame = *SHARED_FRAME_COUNTER + 10;
    return;
  }
  if (gCtrdgPollStopped != 0) {
    return;
  }
  if (gCtrdgPulledOut != 0) {
    return;
  }
  if (*SHARED_FRAME_COUNTER < gCtrdgNextPollFrame) {
    return;
  }
  gCtrdgNextPollFrame = *SHARED_FRAME_COUNTER + 10;
  gCtrdgPulledOut = CTRDG_IsPulledOut();
  if (!Cartridge_HeaderStillMatches_Arm7()) {
    if (gCtrdgFirstPoll != 0) {
      gCtrdgPollStopped = 1;
      return;
    }
    gCtrdgPulledOut = 1;
  }
  gCtrdgFirstPoll = 0;
  if (gCtrdgPulledOut != 0) {
    while (Fifo_PushWord_Arm7(FIFO_TAG_GBA_SLOT, GBA_SLOT_MSG_REMOVED, 0) !=
           0) {
      Thread_NapMilliseconds_Arm7(100);
    }
  }
}
