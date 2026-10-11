#include "sdk/snd.h"
#include "sdk/os.h"

int Sound_FreeCommandCount(void)
{
    int irq = OS_DisableInterrupts();
    int count = 0;
    SndCommand *command = data_022a9c68;

    while (command != NULL) {
        command = command->next;
        count++;
    }
    OS_RestoreInterrupts(irq);
    return count;
}
