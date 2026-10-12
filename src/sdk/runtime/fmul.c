#include "types.h"

u8 *func_02129748(void);
float func_02121e0c(float a, float b, u32 op);
float func_02121ef4(float a, u32 op);

/*
 * Soft-float single-precision multiply: r0 = r0 * r1, as IEEE bit patterns.
 * The operands stay on the stack for the trap wrappers; the operation number
 * (3 = multiply) is part of each trap word. Status word bits as in fadd.c,
 * plus 0x8 underflow (flag) and 0x800 (its trap enable).
 *
 * The symbol table gave this function 0x1ec bytes; the code runs on to
 * 0x02128ac0. The 12 bytes after it swap r0 and r1 and fall into _fsub.
 */

asm void _fmul(void)
{
    stmfd sp!, {r0, r1}
    eor r2, r0, r1
    and r2, r2, #0x80000000
    mov ip, #0xff
    ands r3, ip, r0, lsr #23
    mov r0, r0, lsl #8
    cmpne r3, #0xff
    beq a_exp_zero_or_max
    orr r0, r0, #0x80000000
    ands ip, ip, r1, lsr #23
    mov r1, r1, lsl #8
    cmpne ip, #0xff
    beq b_exp_zero_or_max
    orr r1, r1, #0x80000000
multiply:
    add ip, r3, ip
    umull r1, r3, r0, r1
    cmp r1, #0
    orrne r3, r3, #1
    movs r0, r3
    addpl r0, r0, r0
    subpl ip, ip, #1
    subs ip, ip, #0x7f
    bmi underflow
    cmp ip, #0xfe
    bge overflow
    ands r3, r0, #0xff
    orr r0, r2, r0, lsr #8
    add r0, r0, ip, lsl #23
    addeq sp, sp, #8
    bxeq lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl func_02129748
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r1, [ip]
    tst r1, #0x1000
    bne inexact_trap
    orr r1, r1, #16
    str r1, [ip]
    tst r1, #0x800000
    bne round_toward_minus_inf_or_zero
    tst r1, #0x400000
    bne round_toward_plus_inf
    tst r3, #0x80
    addeq sp, sp, #8
    bxeq lr
    ands r3, r3, #0x7f
    andeqs r3, r0, #1
    addeq sp, sp, #8
    bxeq lr
round_up:
    add r0, r0, #1
    mov r3, r0, lsl #1
    cmn r3, #0x1000000
    addcc sp, sp, #8
    bxcc lr
    tst r1, #0x400
    bne overflow_trap_after_round
    orr r1, r1, #20
    str r1, [ip]
    add sp, sp, #8
    bx lr
round_toward_plus_inf:
    cmp r2, #0
    beq round_up
    add sp, sp, #8
    bx lr
round_toward_minus_inf_or_zero:
    tst r1, #0x400000
    addne sp, sp, #8
    bxne lr
    cmp r2, #0
    bne round_up
    add sp, sp, #8
    bx lr
a_exp_zero_or_max:
    cmp r3, #0
    beq a_zero_or_subnormal
    movs r0, r0, lsl #1
    bne a_nan
    mov ip, r1, lsr #23
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq a_inf_b_exp_zero
    cmp ip, #0xff
    blt return_inf
    cmp r1, #0
    beq return_inf
    b b_nan
a_inf_b_exp_zero:
    cmp r1, #0
    beq signal_invalid
    b return_inf
b_exp_zero_or_max:
    cmp ip, #0
    beq b_zero_or_subnormal
b_inf_or_nan:
    movs r1, r1, lsl #1
    bne b_nan
    b return_inf
a_zero_or_subnormal:
    movs r0, r0, lsl #1
    beq a_zero
    mov r0, r0, lsr #1
    clz r3, r0
    movs r0, r0, lsl r3
    rsb r3, r3, #1
    mov ip, r1, lsr #23
    mov r1, r1, lsl #8
    ands ip, ip, #0xff
    beq b_zero_or_subnormal
    cmp ip, #0xff
    beq b_inf_or_nan
    orr r1, r1, #0x80000000
    b multiply
a_zero:
    mov ip, r1, lsr #23
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq return_zero
    cmp ip, #0xff
    blt return_zero
    cmp r1, #0
    beq signal_invalid
    b b_nan
b_zero_or_subnormal:
    movs r1, r1, lsl #1
    beq return_zero
    mov r1, r1, lsr #1
    clz ip, r1
    movs r1, r1, lsl ip
    rsb ip, ip, #1
    b multiply
underflow:
    stmfd sp!, {r0, r1, r2, ip, lr}
    bl func_02129748
    mov r3, r0
    ldmfd sp!, {r0, r1, r2, ip, lr}
    ldr r3, [r3]
    tst r3, #0x800
    bne underflow_trap
    cmn ip, #24
    beq underflow_shift_24
    bmi underflow_to_zero
    mov r3, r0
    mov r0, r0, lsr #8
    rsb ip, ip, #0
    orr r0, r2, r0, lsr ip
    rsb ip, ip, #24
    movs r1, r3, lsl ip
    addeq sp, sp, #8
    bxeq lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl func_02129748
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #24
    str r3, [ip]
    tst r3, #0x800000
    bne subnormal_toward_minus_inf_or_zero
    tst r3, #0x400000
    bne subnormal_toward_plus_inf
    tst r1, #0x80000000
    addeq sp, sp, #8
    bxeq lr
    movs r1, r1, lsl #1
    andeqs r1, r0, #1
    addne r0, r0, #1
    add sp, sp, #8
    bx lr
subnormal_toward_plus_inf:
    cmp r2, #0
    addeq r0, r0, #1
    add sp, sp, #8
    bx lr
subnormal_toward_minus_inf_or_zero:
    tst r3, #0x400000
    addeq r0, r0, r0, lsr #31
    add sp, sp, #8
    bx lr
underflow_shift_24:
    mov r0, r0, lsl #1
    b underflow_to_tiny
return_inf:
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    add sp, sp, #8
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
    add sp, sp, #8
    bx lr
signal_invalid:
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl func_02129748
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x100
    bne invalid_trap
    orr r3, r3, #1
    str r3, [r2]
return_nan:
    mvn r0, #0x80000000
    add sp, sp, #8
    bx lr
overflow:
    stmfd sp!, {r0, r2, r3, ip, lr}
    bl func_02129748
    mov r1, r0
    ldmfd sp!, {r0, r2, r3, ip, lr}
    ldr r3, [r1]
    tst r3, #0x400
    bne overflow_trap
    orr r3, r3, #20
    str r3, [r1]
    mov r0, #0xff000000
    orr r0, r2, r0, lsr #1
    tst r3, #0x800000
    bne overflow_toward_minus_inf
    tst r3, #0x400000
    subne r0, r0, r2, lsr #31
    add sp, sp, #8
    bx lr
overflow_toward_minus_inf:
    sub r0, r0, #1
    tst r3, #0x400000
    addeq r0, r0, r2, lsr #31
    add sp, sp, #8
    bx lr
underflow_to_tiny:
    movs r1, r0
    mov r0, r2
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl func_02129748
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #24
    str r3, [ip]
    cmp r1, #0
    tst r3, #0x800000
    bne tiny_toward_minus_inf_or_zero
    tst r3, #0x400000
    bne tiny_toward_plus_inf
    cmp r1, #0
    addne r0, r0, #1
    add sp, sp, #8
    bx lr
underflow_to_zero:
    mov r0, r2
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl func_02129748
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x1000
    bne inexact_trap
    orr r3, r3, #24
    str r3, [ip]
    tst r3, #0x800000
    bne tiny_toward_minus_inf_or_zero
    tst r3, #0x400000
    addeq sp, sp, #8
    bxeq lr
    cmp r0, #0
    addeq r0, r0, #1
    add sp, sp, #8
    bx lr
return_zero:
    mov r0, r2
    add sp, sp, #8
    bx lr
tiny_toward_plus_inf:
    cmp r2, #0
    addeq r0, r0, #1
    add sp, sp, #8
    bx lr
tiny_toward_minus_inf_or_zero:
    tst r3, #0x400000
    addeq r0, r0, r0, lsr #31
    add sp, sp, #8
    bx lr
overflow_trap_after_round:
    add sp, sp, #8
    sub r0, r0, #0x60000000
    stmfd sp!, {lr}
    mov r1, #0x10000003
    bl func_02121ef4
    ldmfd sp!, {lr}
    bx lr
overflow_trap:
    add sp, sp, #8
    sub ip, ip, #0xc0
    orr r0, r2, r0, lsr #8
    add r0, r0, ip, lsl #23
    stmfd sp!, {lr}
    mov r1, #0x10000003
    bl func_02121ef4
    ldmfd sp!, {lr}
    bx lr
underflow_trap:
    add sp, sp, #8
    add ip, ip, #0xc0
    orr r0, r2, r0, lsr #8
    add r0, r0, ip, lsl #23
    stmfd sp!, {lr}
    mov r1, #0x20000003
    bl func_02121ef4
    ldmfd sp!, {lr}
    bx lr
invalid_trap:
    ldmfd sp!, {r0, r1}
    stmfd sp!, {lr}
    mov r2, #0x4000003
    bl func_02121e0c
    ldmfd sp!, {lr}
    bx lr
inexact_trap:
    add sp, sp, #8
    stmfd sp!, {lr}
    mov r1, #0x40000003
    bl func_02121ef4
    ldmfd sp!, {lr}
    bx lr
}
