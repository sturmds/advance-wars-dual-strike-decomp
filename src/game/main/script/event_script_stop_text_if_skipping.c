#include "types.h"

extern u8 data_022900b8;
void Text_StopPrinting(void);

/* data_022900b8 == 1 makes the event commands skip their sounds and waits
 * (guess: events are being skipped). */
void EventScript_StopTextIfSkipping(void)
{
    if (data_022900b8 != 1) {
        return;
    }
    Text_StopPrinting();
}
