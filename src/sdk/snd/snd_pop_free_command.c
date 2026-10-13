#include "sdk/snd.h"
#include "sdk/os.h"

SndCommand* data_022a9c68; /* free list head */

SndCommand* SND_PopFreeCommand(void) {
  int irq = OS_DisableInterrupts();
  SndCommand* command = data_022a9c68;

  if (command == NULL) {
    OS_RestoreInterrupts(irq);
    return NULL;
  }
  data_022a9c68 = command->next;
  if (data_022a9c68 == NULL) {
    data_022a9c78 = NULL;
  }
  OS_RestoreInterrupts(irq);
  return command;
}
