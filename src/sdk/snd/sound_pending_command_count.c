#include "sdk/snd.h"
#include "sdk/os.h"

int Sound_PendingCommandCount(void) {
  int irq = OS_DisableInterrupts();
  int count = 0;
  SndCommand* command = data_022a9c70;

  while (command != NULL) {
    command = command->next;
    count++;
  }
  OS_RestoreInterrupts(irq);
  return count;
}
