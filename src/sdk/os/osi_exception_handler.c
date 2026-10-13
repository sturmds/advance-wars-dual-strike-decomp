#include "types.h"

extern u32 data_022a9b84;
extern void func_0210e6f8(void);

#ifdef __MWERKS__
/*
 * Exception vector target. Calls the user handler (data_022a9b84) if one is
 * set, switches to a stack at 0x02000000, and saves the context for aborts
 * (mode 0x17) and undefined instructions (mode 0x1b). It then spins forever;
 * the register restore after the loops is never reached.
 */
asm void OSi_ExceptionHandler(void)
{
    ldr ip, =data_022a9b84
    ldr ip, [ip]
    cmp ip, #0
    movne lr, pc
    bxne ip
    ldr ip, =0x02000000
    stmdb ip!, {r0, r1, r2, r3, sp, lr}
    and r0, sp, #1
    mov sp, ip
    mrs r1, CPSR
    and r1, r1, #0x1f
    teq r1, #0x17
    bne not_data_abort
    bl func_0210e6f8
    b after_context
not_data_abort:
    teq r1, #0x1b
    bne after_context
    bl func_0210e6f8
after_context:
    ldr ip, =data_022a9b84
    ldr ip, [ip]
    cmp ip, #0
hang:
    beq hang
spin:
    nop
    b spin
    ldmia sp!, {r0, r1, r2, r3, ip, lr}
    mov sp, ip
    bx lr
}
#else
#error "OSi_ExceptionHandler: switching stacks and reading the CPU mode in the exception vector (mrs) has no portable C equivalent"
#endif
