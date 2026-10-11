#include "types.h"
#include "game/sound_stream.h"

extern LinkList data_022a8ab8;         /* refill requests waiting for the sound thread */

int func_0210eb0c(void);
void func_0210eb20(int irqState);

/* Takes the oldest waiting refill request, or returns NULL when none is
 * queued. The request's stream has one request fewer pending. Interrupts
 * are off while the list changes. */
SoundStreamRequest *SoundStreamRequest_PopPending(void)
{
    int irqState = func_0210eb0c();
    SoundStreamRequest *request = LinkList_GetNext(&data_022a8ab8, NULL);

    if (request != NULL) {
        LinkList_Remove(&data_022a8ab8, request);
        request->stream->pendingRequests--;
    }
    func_0210eb20(irqState);
    return request;
}
