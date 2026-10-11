#include "types.h"

int OS_EnableInterrupts(void);
void func_0210eb70(void);

void OSi_IdleThreadProc(void)
{
    OS_EnableInterrupts();
    while (TRUE) {
        func_0210eb70();
    }
}
