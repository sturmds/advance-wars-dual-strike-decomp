#include "types.h"
#include "game/Unk_0229d8e0.h"

void Mem_CopyBytes(void *src, void *dest, int size);

/* Copies the oldest queued action to dest. Returns 0 when the queue is empty. */
int LinkActionQueue_Pop(UnitAction *dest)
{
    if (data_0229d8e0.unk_180 == data_0229d8e0.unk_181) {
        return 0;
    }
    Mem_CopyBytes(&data_0229d8e0.slots[data_0229d8e0.unk_180], dest, sizeof(UnitAction));
    data_0229d8e0.unk_180++;
    if (data_0229d8e0.unk_180 == 16) {
        data_0229d8e0.unk_180 = 0;
    }
    return 1;
}
