#include "types.h"

void OS_SpinWait(int);

void OSi_WaitByLoop(void) { OS_SpinWait(0x1000); }
