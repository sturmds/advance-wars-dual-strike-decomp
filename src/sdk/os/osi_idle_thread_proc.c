#include "types.h"

int OS_EnableInterrupts(void);
void OS_Halt(void);

void OSi_IdleThreadProc(void)
{
    OS_EnableInterrupts();
    while (TRUE) {
        OS_Halt();
    }
}
