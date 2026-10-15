#include "types.h"
#include "sdk/os.h"

void OSi_WaitByLoop(void) { OS_SpinWait(0x1000); }
