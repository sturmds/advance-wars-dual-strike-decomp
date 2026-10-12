#include "types.h"
#include "sdk/card.h"
#include "sdk/gx.h"

/* Counts down the scanlines since the last call, handling the wrap from the last line to 0. Returns whether time is left. */
int ScanlineTimer_HasTimeLeft(CARDiLineTimer *timer)
{
    u16 line = *REG_VCOUNT;
    int elapsed = line - timer->lastLine;

    if (elapsed != 0) {
        timer->lastLine = line;
        timer->remaining -= elapsed;
        if (elapsed < 0) {
            timer->remaining -= GAMECARD_TIMER_FRAME_LINES;
        }
    }
    return timer->remaining > 0;
}
