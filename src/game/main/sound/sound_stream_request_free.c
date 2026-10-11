#include "types.h"
#include "game/sound_stream.h"

extern LinkList data_022a8aac;         /* unused refill requests */

int OS_DisableInterrupts(void);
void OS_RestoreInterrupts(int irqState);

/* Gives a refill request back to the unused list. Interrupts are off while
 * the list changes. */
void SoundStreamRequest_Free(SoundStreamRequest *request)
{
    int irqState = OS_DisableInterrupts();

    LinkList_Append(&data_022a8aac, request);
    OS_RestoreInterrupts(irqState);
}
