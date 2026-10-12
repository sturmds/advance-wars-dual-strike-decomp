#include "types.h"
#include "game/sound_stream.h"

extern LinkList
    data_022a8ab8; /* refill requests waiting for the sound thread */

int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int irqState);

/* Takes the oldest waiting refill request, or returns NULL when none is
 * queued. The request's stream has one request fewer pending. Interrupts
 * are off while the list changes. */
SoundStreamRequest* SoundStreamRequest_PopPending(void) {
  int irqState = OS_DisableInterrupts();
  SoundStreamRequest* request = LinkList_GetNext(&data_022a8ab8, NULL);

  if (request != NULL) {
    LinkList_Remove(&data_022a8ab8, request);
    request->stream->pendingRequests--;
  }
  OS_RestoreInterrupts(irqState);
  return request;
}
