#include "types.h"
#include "sdk/os_arm7.h"

extern OSHeapInfo *data_arm7_03807a54[];    /* heap info of each arena, by arena id */

/* ARM7 copy of OS_SetCurrentHeap. Picks the arena's default heap and returns the previous one. */
s32 Heap_SelectDefault_Arm7(OSArenaId id, s32 heap)
{
    int irqState = Cpu_MaskIrq_Arm7();
    OSHeapInfo *info = data_arm7_03807a54[id];
    s32 previous = info->currentHeap;

    info->currentHeap = heap;
    Cpu_RestoreIrqBit_Arm7(irqState);
    return previous;
}
