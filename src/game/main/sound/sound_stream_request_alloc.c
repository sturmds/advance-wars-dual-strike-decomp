#include "types.h"
#include "game/sound_stream.h"

extern LinkList data_022a8aac;         /* unused refill requests */

int func_0210eb0c(void);
void func_0210eb20(int irqState);

/* Takes a refill request off the unused list, or returns NULL when none is
 * left. Interrupts are off while the list changes. */
SoundStreamRequest *SoundStreamRequest_Alloc(void)
{
    int irqState = func_0210eb0c();
    SoundStreamRequest *request = LinkList_GetNext(&data_022a8aac, NULL);

    if (request != NULL) {
        LinkList_Remove(&data_022a8aac, request);
    }
    func_0210eb20(irqState);
    return request;
}
