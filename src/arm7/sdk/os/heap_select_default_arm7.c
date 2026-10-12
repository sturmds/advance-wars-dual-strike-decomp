#include "types.h"
#include "sdk/os_arm7.h"

extern OSHeapInfo *data_arm7_03807a54[];    /* heap info of each arena, by arena id */

int func_arm7_037fdb80(void);
void func_arm7_037fdb94(int state);

/* ARM7 copy of OS_SetCurrentHeap. Picks the arena's default heap and returns the previous one. */
s32 Heap_SelectDefault_Arm7(ArenaRegionArm7 id, s32 heap)
{
    int irqState = func_arm7_037fdb80();
    OSHeapInfo *info = data_arm7_03807a54[id];
    s32 previous = info->currentHeap;

    info->currentHeap = heap;
    func_arm7_037fdb94(irqState);
    return previous;
}
