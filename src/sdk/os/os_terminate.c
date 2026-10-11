#include "types.h"
#include "sdk/os.h"

void OS_Halt(void);

void OS_Terminate(void)
{
    while (TRUE) {
        OS_DisableInterrupts();
        OS_Halt();
    }
}
