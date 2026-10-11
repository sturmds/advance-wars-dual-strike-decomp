#include "sdk/snd.h"
#include "sdk/os.h"

/* Adds a filled-in command to the end of the queued list. */
void Sound_LinkCommandAtTail(SndCommand *command)
{
    int irq = OS_DisableInterrupts();

    if (data_022a9c74 == NULL) {
        data_022a9c74 = command;
        data_022a9c70 = command;
    } else {
        data_022a9c74->next = command;
        data_022a9c74 = command;
    }
    command->next = NULL;
    OS_RestoreInterrupts(irq);
}
