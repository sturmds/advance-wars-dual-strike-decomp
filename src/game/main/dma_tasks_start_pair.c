#include "types.h"

void func_0200371c(const void *src1, const void *src2, volatile void *dest1, volatile void *dest2,
                   u16 unk34, u16 unk36, void (*callback1)(void), void (*callback2)(void));

/* Starts one DMA task per channel (func_0200371c): src1 feeds dest1 and src2
 * feeds dest2. The callbacks run each time a channel restarts. */
void DmaTasks_StartPair(const void *src1, const void *src2, volatile void *dest1, volatile void *dest2,
                   void (*callback1)(void), void (*callback2)(void))
{
    func_0200371c(src1, src2, dest1, dest2, 0, 0, callback1, callback2);
}
