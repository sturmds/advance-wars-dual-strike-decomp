#include "types.h"

s8 func_02017580(const void *descriptor);   /* finds the slot (of 30) using descriptor, calls func_020175e0 on it */
void Task_DestroyByScript(const void *descriptor); /* calls Task_DestroyVisitor on each of 80 tasks running descriptor */

extern u8 data_021583b4[];
extern u8 data_021587a8[];

/* Runs before a hint or terms script starts: passes two descriptors (each
 * starting with a handler) to the two slot systems, probably to stop them. */
void func_0201e954(void)
{
    func_02017580(data_021583b4);
    Task_DestroyByScript(data_021587a8);
}
