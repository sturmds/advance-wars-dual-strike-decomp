#include "types.h"
#include "game/sound_stream.h"

extern LinkList data_022a8aac;         /* unused refill requests */

int func_0210eb0c(void);
void func_0210eb20(int irqState);

/* Gives a refill request back to the unused list. Interrupts are off while
 * the list changes. */
void SoundStreamRequest_Free(SoundStreamRequest *request)
{
    int irqState = func_0210eb0c();

    LinkList_Append(&data_022a8aac, request);
    func_0210eb20(irqState);
}
