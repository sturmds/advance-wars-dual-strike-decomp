#include "sdk/os_arm7.h"

extern OSIrqFunction data_arm7_03807750[];   /* handler table, one entry per interrupt bit */

#ifdef __MWERKS__
/*
 * ARM7 copy of OS_IrqHandler, the IRQ vector body. With IME on, picks the
 * lowest bit set in IE & IF, acknowledges it in IF, and calls its handler
 * from the table with lr pointing at Irq_WakeAndSwitchOnExit_Arm7 (the thread switch
 * on the way out). Returns right away when IME is off or no enabled
 * interrupt is pending. The ARM7 has no clz, so it scans the bits upwards.
 */
asm void Irq_DispatchLowestPending_Arm7(void)
{
    stmfd sp!, {lr}
    mov r12, #0x4000000
    add r12, r12, #0x210
    ldr r1, [r12, #-8]
    cmp r1, #0
    ldmeqfd sp!, {pc}
    ldmia r12, {r1, r2}
    ands r1, r1, r2
    ldmeqfd sp!, {pc}
    mov r3, #1
    mov r0, #0
next_bit:
    ands r2, r1, r3, lsl r0
    addeq r0, r0, #1
    beq next_bit
    str r2, [r12, #4]
    ldr r1, =data_arm7_03807750
    ldr r0, [r1, r0, lsl #2]
    ldr lr, =Irq_WakeAndSwitchOnExit_Arm7
    bx r0
}
#else
/* The assembly calls the handler with lr at Irq_WakeAndSwitchOnExit_Arm7,
   which ends by popping the lr pushed here; calling one after the other
   does the same. */
void Irq_DispatchLowestPending_Arm7(void) {
  u32 pending;
  int bit;

  if (*(vu32*)REG_IME == 0) { /* a word read, as in the assembly */
    return;
  }
  pending = *REG_IE & *REG_IF;
  if (pending == 0) {
    return;
  }
  for (bit = 0; (pending & (1u << bit)) == 0; bit++) {
  }
  *REG_IF = 1u << bit;
  data_arm7_03807750[bit]();
  Irq_WakeAndSwitchOnExit_Arm7();
}
#endif
