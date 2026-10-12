#include "types.h"
#include "sdk/card_arm7.h"
#include "sdk/gx.h"

int func_arm7_037fdbe4(int arg);
int ScanlineTimer_HasTimeLeft(CARDiLineTimer *timer);
int CardSave_IsChipIdle(void);

/* Waits minWait scanlines, then polls the save chip until it is idle or maxWait scanlines have passed; a timeout sets CARD_RESULT_TIMEOUT. */
void CardSave_WaitUntilIdle(int minWait, int maxWait)
{
    CARDiLineTimer minTimer;
    CARDiLineTimer maxTimer;
    int ready = 1;

    if (minWait + maxWait != 0) {
        if (minWait != 0) {
            minTimer.lastLine = *REG_VCOUNT;
            minTimer.remaining = minWait;
            while (ScanlineTimer_HasTimeLeft(&minTimer)) {
                func_arm7_037fdbe4(0);
            }
        }
        if (maxWait != 0) {
            maxTimer.lastLine = *REG_VCOUNT;
            maxTimer.remaining = maxWait - minWait;
            ready = 0;
            while (ready == 0 && ScanlineTimer_HasTimeLeft(&maxTimer)) {
                func_arm7_037fdbe4(0);
                ready = CardSave_IsChipIdle();
            }
        } else {
            ready = CardSave_IsChipIdle();
        }
    }
    if (ready == 0) {
        data_arm7_03809280.cmd->result = CARD_RESULT_TIMEOUT;
    }
}
