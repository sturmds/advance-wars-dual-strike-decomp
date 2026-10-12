#include "types.h"
#include "game/sound_stream.h"

extern LinkList data_022a8aac; /* unused refill requests */

int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int irqState);

/* Takes a refill request off the unused list, or returns NULL when none is
 * left. Interrupts are off while the list changes. */
SoundStreamRequest* SoundStreamRequest_Alloc(void) {
  int irqState = OS_DisableInterrupts();
  SoundStreamRequest* request = LinkList_GetNext(&data_022a8aac, NULL);

  if (request != NULL) {
    LinkList_Remove(&data_022a8aac, request);
  }
  OS_RestoreInterrupts(irqState);
  return request;
}
