#include "types.h"

extern u8 gEventSkipState;
void Text_StopPrinting(void);

/* gEventSkipState == 1 makes the event commands skip their sounds and waits
 * (guess: events are being skipped). */
void EventScript_StopTextIfSkipping(void)
{
    if (gEventSkipState != 1) {
        return;
    }
    Text_StopPrinting();
}
