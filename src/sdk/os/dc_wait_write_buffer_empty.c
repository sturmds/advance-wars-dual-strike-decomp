#include "types.h"

#ifdef __MWERKS__
/* Returns once the write buffer has drained to memory. */
asm void DC_WaitWriteBufferEmpty(void)
{
    mov r0, #0
    mcr p15, 0, r0, c7, c10, 4
    bx lr
}
#else
#error "DC_WaitWriteBufferEmpty: draining the write buffer (mcr) has no portable C equivalent"
#endif
