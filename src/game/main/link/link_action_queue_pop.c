#include "types.h"
#include "game/Unk_0229d8e0.h"

void Mem_CopyBytes(void *src, void *dest, int size);

/* Copies the oldest queued action to dest. Returns 0 when the queue is empty. */
int LinkActionQueue_Pop(UnitAction *dest)
{
    if (data_0229d8e0.readIndex == data_0229d8e0.writeIndex) {
        return 0;
    }
    Mem_CopyBytes(&data_0229d8e0.slots[data_0229d8e0.readIndex], dest, sizeof(UnitAction));
    data_0229d8e0.readIndex++;
    if (data_0229d8e0.readIndex == 16) {
        data_0229d8e0.readIndex = 0;
    }
    return 1;
}
