#include "types.h"
#include "sdk/ctrdg.h"
#include "sdk/pm.h"

BOOL CTRDG_IsPulledOut(void);
BOOL func_arm7_0380660c(void);
int func_arm7_037fe1cc(u32 tag, u32 data, int err);   /* PXI_SendWordByFifo: 0 once queued */
void Thread_NapMilliseconds_Arm7(u32 msec);

extern u32 gCtrdgFirstPoll;       /* 1 until the first check ran */
extern u32 gCtrdgNextPollFrame;   /* VBlank count of the next check; 0xffffffff before the first call */
extern u32 gCtrdgPollStopped;     /* set when no cartridge was there at the first check */
extern u32 gCtrdgPulledOut;       /* set once the cartridge was found pulled out */

/*
 * Checks the GBA slot every 10 frames. Once the cartridge is found pulled
 * out, tells the ARM9 and stops checking. If there was no cartridge at the
 * first check, it stops without telling the ARM9.
 */
void Cartridge_CheckRemovedEveryTenFrames(void)
{
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
    if (!func_arm7_0380660c()) {
        if (gCtrdgFirstPoll != 0) {
            gCtrdgPollStopped = 1;
            return;
        }
        gCtrdgPulledOut = 1;
    }
    gCtrdgFirstPoll = 0;
    if (gCtrdgPulledOut != 0) {
        while (func_arm7_037fe1cc(FIFO_TAG_GBA_SLOT, GBA_SLOT_MSG_REMOVED, 0) != 0) {
            Thread_NapMilliseconds_Arm7(100);
        }
    }
}
