#include "types.h"

void OSi_WaitByLoop(void);
int OSi_DoTryLockByWord(int a, int b, int c, int d);

void OSi_DoLockByWord(int a, int b, int c, int d)
{
    while (OSi_DoTryLockByWord(a, b, c, d) > 0) {
        OSi_WaitByLoop();
    }
}
