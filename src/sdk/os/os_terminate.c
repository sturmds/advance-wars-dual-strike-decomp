#include "types.h"
#include "sdk/os.h"

void func_0210eb70(void);

void OS_Terminate(void)
{
    while (TRUE) {
        OS_DisableInterrupts();
        func_0210eb70();
    }
}
