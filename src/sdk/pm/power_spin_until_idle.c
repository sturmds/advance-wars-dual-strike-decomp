#include "sdk/pm.h"

extern volatile PowerCommandState gPMWork;

/* Waits until the command in flight has finished. Always returns 0 (needed to match). */
int Power_SpinUntilIdle(void)
{
    int lock;

    while ((lock = gPMWork.lock) != 0) {
    }
    return lock;
}
