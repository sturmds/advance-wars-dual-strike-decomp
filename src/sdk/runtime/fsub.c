#include "types.h"

extern void _fadd(void);
u32 *__fp_status(void);
float __fp_trap_ff(float a, float b, u32 op);
float __fp_trap_f(float a, u32 op);

/*
 * Soft-float single-precision subtract: r0 = r0 - r1, as IEEE bit patterns.
 * Operands of different sign go to the add path in _fadd, with b negated.
 * The operands and the operation number (2 = subtract) stay on the stack for
 * the trap wrappers. The status word bits are listed in fadd.c.
 */
asm void _fsub(void)
{
    mov r2, #2
    stmfd sp!, {r0, r1, r2}
    eors r2, r0, r1
    eormi r1, r1, #0x80000000
    dcd 0x4afff881 /* bmi _fadd+0x14, encoded by hand: the assembler takes no symbol offsets */
    subs ip, r0, r1
    eorcc ip, ip, #0x80000000
    subcc r0, r0, ip
    addcc r1, r1, ip
    mov r2, #0x80000000
    mov r3, r0, lsr #23
    orr r0, r2, r0, lsl #8
    ands ip, r3, #0xff
    cmpne ip, #0xff
    beq a_exp_zero_or_max
    mov ip, r1, lsr #23
    orr r1, r2, r1, lsl #8
    ands r2, ip, #0xff
    beq b_exp_zero
align_b:
    subs ip, r3, ip
    beq equal_exponents
    cmp ip, #28
    movge ip, #31
    rsb r2, ip, #32
    movs r2, r1, lsl r2
    mov r1, r1, lsr ip
    orrne r1, r1, #1
    subs r0, r0, r1
    bpl normalize_difference
    ands r1, r0, #0xff
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    addeq sp, sp, #12
    bxeq lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #16
    str r3, [ip]
    tst r3, #0x800000
    bne round_toward_minus_inf_or_zero
    tst r3, #0x400000
    bne round_toward_plus_inf
    tst r1, #0x80
    addeq sp, sp, #12
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    add sp, sp, #12
    bx lr
round_toward_plus_inf:
    cmp r0, #0
    addpl r0, r0, #1
    add sp, sp, #12
    bx lr
round_toward_minus_inf_or_zero:
    tst r3, #0x400000
    addne sp, sp, #12
    bxne lr
    cmp r0, #0
    addmi r0, r0, #1
    add sp, sp, #12
    bx lr
equal_exponents:
    subs r0, r0, r1
    beq return_zero
    mov r2, r3, lsl #23
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
    cmp r3, #0
    bgt pack_exact
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    add sp, sp, #12
    bx lr
pack_exact:
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    add sp, sp, #12
    bx lr
normalize_difference:
    mov r2, r3, lsl #23
    and r2, r2, #0x80000000
    bic r3, r3, #0x100
    clz ip, r0
    movs r0, r0, lsl ip
    sub r3, r3, ip
    cmp r3, #0
    bgt round_normalized
    rsb r3, r3, #9
    orr r0, r2, r0, lsr r3
    add sp, sp, #12
    bx lr
round_normalized:
    ands r1, r0, #0xff
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    addeq sp, sp, #12
    bxeq lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #16
    str r3, [ip]
    tst r3, #0x800000
    bne round_toward_minus_inf_or_zero
    tst r3, #0x400000
    bne round_toward_plus_inf
    tst r1, #0x80
    addeq sp, sp, #12
    bxeq lr
    ands r1, r1, #0x7f
    andeqs r1, r0, #1
    addne r0, r0, #1
    add sp, sp, #12
    bx lr
a_exp_zero_or_max:
    cmp r3, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    ands r3, r3, #0xff
    beq a_zero_or_subnormal
    movs r0, r0, lsl #1
    bne a_nan
    mov ip, r1, lsr #23
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq return_inf
    cmp ip, #0xff
    blt return_inf
    cmp r1, #0
    beq signal_invalid
    b b_nan
b_exp_zero:
    cmp ip, #0x100
    movge r2, #0x80000000
    movlt r2, #0
    and r3, r3, #0xff
    ands ip, ip, #0xff
    beq b_zero_or_subnormal
b_inf_or_nan:
    eor r2, r2, #0x80000000
    movs r1, r1, lsl #1
    bne b_nan
    b return_inf
a_zero_or_subnormal:
    movs r0, r0, lsl #1
    beq a_zero
    mov r0, r0, lsr #1
    mov r3, #1
    mov ip, r1, lsr #23
    mov r1, r1, lsl #8
    ands ip, ip, #0xff
    beq b_zero_or_subnormal
    cmp ip, #0xff
    beq b_inf_or_nan
    orr r1, r1, #0x80000000
    orr r3, r3, r2, lsr #23
    orr ip, ip, r2, lsr #23
    b align_b
a_zero:
    mov r3, r1, lsr #23
    mov r0, r1, lsl #9
    ands r2, r3, #0xff
    beq a_zero_b_exp_zero
    cmp r2, #0xff
    blt return_b
    cmp r0, #0
    bne a_zero_b_nan
    b return_inf
a_zero_b_exp_zero:
    cmp r0, #0
    beq return_zero
    b return_b
b_zero_or_subnormal:
    movs r1, r1, lsl #1
    beq return_a
    mov r1, r1, lsr #1
    mov ip, #1
    orr ip, ip, r2, lsr #23
    orr r3, r3, r2, lsr #23
    b align_b
return_b:
    mov r0, r1
    add sp, sp, #12
    bx lr
return_a:
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    orr r0, r2, r0, lsr #9
    orr r0, r0, r3, lsl #23
    add sp, sp, #12
    bx lr
return_zero:
    mov r0, #0
    stmfd sp!, {r0, r1, r2, ip, lr}
    bl __fp_status
    mov r3, r0
    ldmfd sp!, {r0, r1, r2, ip, lr}
    ldr r3, [r3]
    tst r3, #0x800000
    addeq sp, sp, #12
    bxeq lr
    tst r3, #0x400000
    addne sp, sp, #12
    bxne lr
    mov r0, #0x80000000
    add sp, sp, #12
    bx lr
    cmp r0, #0
    subges r3, r3, #1
    add r0, r0, r0
    mov r0, r0, lsr #9
    orr r0, r0, r3, lsl #23
    add sp, sp, #12
    bx lr
a_zero_b_nan:
    mov r3, r0
    mvn r0, #0x80000000
    tst r3, #0x80000000
    addne sp, sp, #12
    bxne lr
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    orr r3, r3, #1
    str r3, [r2]
    add sp, sp, #12
    bx lr
return_inf:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    add sp, sp, #12
    bx lr
a_nan:
    tst r0, #0x80000000
    beq signal_invalid
    mov ip, r1, lsr #23
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq return_nan
    cmp ip, #0xff
    blt return_nan
    cmp r1, #0
    beq return_nan
b_nan:
    tst r1, #0x80000000
    beq signal_invalid
    mvn r0, #0x80000000
    add sp, sp, #12
    bx lr
signal_invalid:
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl __fp_status
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x100
    bne invalid_trap
    orr r3, r3, #1
    str r3, [r2]
return_nan:
    mvn r0, #0x80000000
    add sp, sp, #12
    bx lr
inexact_trap:
    add sp, sp, #12
    stmfd sp!, {lr}
    mov r1, #0x40000002
    bl __fp_trap_f
    ldmfd sp!, {lr}
    bx lr
invalid_trap:
    ldmfd sp!, {r0, r1, r2}
    stmfd sp!, {lr}
    mov ip, #0x4000000
    orr r2, r2, ip
    bl __fp_trap_ff
    ldmfd sp!, {lr}
    bx lr
}
