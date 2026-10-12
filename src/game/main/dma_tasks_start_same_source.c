#include "types.h"

void DmaTasks_Start(const void* src1, const void* src2, volatile void* dest1,
                    volatile void* dest2, u16 unk34, u16 unk36,
                    void (*callback1)(void), void (*callback2)(void));

/* DmaTasks_Start with one source and one callback for both tasks. The
 * window wipes pass the main and sub WIN0H registers as dest and dest2. */
void DmaTasks_StartSameSource(const void* src, volatile void* dest,
                              volatile void* dest2, u16 unk34, u16 unk36,
                              void (*callback)(void)) {
  DmaTasks_Start(src, src, dest, dest2, unk34, unk36, callback, callback);
}
