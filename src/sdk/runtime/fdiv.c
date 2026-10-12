#include "types.h"

u8 *func_02129748(void);
float func_02121e0c(float a, float b, u32 op);
float func_02121ef4(float a, u32 op);

/*
 * Soft-float single-precision divide: r0 = r0 / r1, as IEEE bit patterns.
 * The quotient starts from a reciprocal estimate of the divisor, read from a
 * 256-byte table inside the function, refined and multiplied by the dividend.
 * The operands stay on the stack for the trap wrappers; the operation number
 * (4 = divide) is part of each trap word. Status word bits as in fadd.c and
 * fmul.c, plus 0x2 divide by zero (flag) and 0x200 (its trap enable).
 *
 * The symbol table gave this function 0x380 bytes; the code and its literal
 * pool run on to 0x0212839c.
 */

asm void _fdiv(void)
{
    stmfd sp!, {r0, r1, lr}
    mov ip, #0xff
    ands r3, ip, r0, lsr #23
    cmpne r3, #0xff
    beq a_exp_zero_or_max
    ands ip, ip, r1, lsr #23
    cmpne ip, #0xff
    beq b_exp_zero_or_max
    orr r1, r1, #0x800000
    orr r0, r0, #0x800000
    bic r2, r0, #0xff000000
    bic lr, r1, #0xff000000
divide:
    cmp r2, lr
    movcc r2, r2, lsl #1
    subcc r3, r3, #1
    teq r0, r1
    sub r0, pc, #4 /* reciprocal_table - 0x100: the index has the implicit bit set */
    ldrb r1, [r0, lr, lsr #15]
    rsb lr, lr, #0
    mov r0, lr, asr #1
    mul r0, r1, r0
    add r0, r0, #0x80000000
    mov r0, r0, lsr #6
    mul r0, r1, r0
    mov r0, r0, lsr #14
    mul r1, lr, r0
    sub ip, r3, ip
    mov r1, r1, lsr #12
    mul r1, r0, r1
    mov r0, r0, lsl #14
    add r0, r0, r1, lsr #15
    umull r1, r0, r2, r0
    mov r3, r0
    orrmi r0, r0, #0x80000000
    adds ip, ip, #0x7e
    bmi underflow
    cmp ip, #0xfe
    bge overflow
    add r0, r0, ip, lsl #23
    rsb lr, lr, #0
    mul r1, lr, r3
    cmp r1, r2, lsl #23
    addeq sp, sp, #8
    ldmeqfd sp!, {lr}
    bxeq lr
    add r1, r1, lr
    cmp r1, r2, lsl #23
    beq exact_plus_one
    addmi r0, r0, #1
    subpl r1, r1, lr
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl func_02129748
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
    add r1, lr, r1, lsl #1
    cmp r1, r2, lsl #24
    addpl sp, sp, #8
    ldmplfd sp!, {lr}
    bxpl lr
round_up:
    add r0, r0, #1
    mov lr, r0, lsl #1
    cmn lr, #0x1000000
    addcc sp, sp, #8
    ldmccfd sp!, {lr}
    bxcc lr
    tst r3, #0x400
    bne overflow_trap_after_round
    orr r3, r3, #20
    str r3, [ip]
    add sp, sp, #8
    ldmfd sp!, {lr}
    bx lr
    /* About 0x100 / (1 + i / 256) for divisor mantissa top bits i, four bytes per word, low byte first */
reciprocal_table:
    dcd 0xfdfeffff
    dcd 0xf9fafbfc
    dcd 0xf5f6f7f8
    dcd 0xf1f2f3f4
    dcd 0xeeeff0f0
    dcd 0xeaebeced
    dcd 0xe7e8e9ea
    dcd 0xe4e5e6e6
    dcd 0xe1e2e2e3
    dcd 0xdedfdfe0
    dcd 0xdbdcdcdd
    dcd 0xd8d9d9da
    dcd 0xd5d6d7d7
    dcd 0xd2d3d4d4
    dcd 0xd0d0d1d2
    dcd 0xcdcececf
    dcd 0xcbcbcccc
    dcd 0xc8c9c9ca
    dcd 0xc6c6c7c8
    dcd 0xc3c4c5c5
    dcd 0xc1c2c2c3
    dcd 0xbfbfc0c0
    dcd 0xbdbdbebe
    dcd 0xbabbbcbc
    dcd 0xb8b9b9ba
    dcd 0xb6b7b7b8
    dcd 0xb4b5b5b6
    dcd 0xb2b3b3b4
    dcd 0xb0b1b1b2
    dcd 0xafafafb0
    dcd 0xadadaeae
    dcd 0xababacac
    dcd 0xa9aaaaaa
    dcd 0xa7a8a8a9
    dcd 0xa6a6a7a7
    dcd 0xa4a4a5a5
    dcd 0xa2a3a3a4
    dcd 0xa1a1a2a2
    dcd 0x9fa0a0a0
    dcd 0x9e9e9e9f
    dcd 0x9c9d9d9d
    dcd 0x9b9b9b9c
    dcd 0x999a9a9a
    dcd 0x98989999
    dcd 0x96979798
    dcd 0x95959696
    dcd 0x94949495
    dcd 0x92939393
    dcd 0x91919292
    dcd 0x90909191
    dcd 0x8f8f8f90
    dcd 0x8d8e8e8e
    dcd 0x8c8c8d8d
    dcd 0x8b8b8c8c
    dcd 0x8a8a8a8b
    dcd 0x8989898a
    dcd 0x88888888
    dcd 0x86878787
    dcd 0x85868686
    dcd 0x84858585
    dcd 0x83838484
    dcd 0x82828383
    dcd 0x81818282
    dcd 0x80808181
round_toward_minus_inf_or_zero:
    tst r3, #0x400000
    addne sp, sp, #8
    ldmnefd sp!, {lr}
    bxne lr
    cmp r0, #0
    bmi round_up
    add sp, sp, #8
    ldmfd sp!, {lr}
    bx lr
round_toward_plus_inf:
    cmp r0, #0
    bpl round_up
    add sp, sp, #8
    ldmfd sp!, {lr}
    bx lr
a_exp_zero_or_max:
    eor lr, r0, r1
    and lr, lr, #0x80000000
    cmp r3, #0
    beq a_zero_or_subnormal
    movs r0, r0, lsl #9
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
b_exp_zero_or_max:
    eor lr, r0, r1
    and lr, lr, #0x80000000
    cmp ip, #0
    beq b_zero_or_subnormal
b_inf_or_nan:
    movs r1, r1, lsl #9
    bne b_nan
    b return_zero
a_zero_or_subnormal:
    movs r2, r0, lsl #9
    beq a_zero
    clz r3, r2
    movs r2, r2, lsl r3
    rsb r3, r3, #0
    mov r2, r2, lsr #8
    ands ip, ip, r1, lsr #23
    beq a_subnormal_b_exp_zero
    cmp ip, #0xff
    beq b_inf_or_nan
    orr r1, r1, #0x800000
    bic lr, r1, #0xff000000
    b divide
a_zero:
    mov ip, r1, lsr #23
    mov r1, r1, lsl #9
    ands ip, ip, #0xff
    beq a_zero_b_exp_zero
    cmp ip, #0xff
    blt return_zero
    cmp r1, #0
    beq return_zero
    b b_nan
a_zero_b_exp_zero:
    cmp r1, #0
    beq signal_invalid
    b return_zero
b_zero_or_subnormal:
    movs ip, r1, lsl #9
    beq divide_by_zero
    mov lr, ip
    clz ip, lr
    movs lr, lr, lsl ip
    rsb ip, ip, #0
    mov lr, lr, lsr #8
    orr r0, r0, #0x800000
    bic r2, r0, #0xff000000
    b divide
a_subnormal_b_exp_zero:
    movs ip, r1, lsl #9
    beq divide_by_zero
    mov lr, ip
    clz ip, lr
    movs lr, lr, lsl ip
    rsb ip, ip, #0
    mov lr, lr, lsr #8
    b divide
underflow:
    stmfd sp!, {r0, r2, r3, ip, lr}
    bl func_02129748
    mov r1, r0
    ldmfd sp!, {r0, r2, r3, ip, lr}
    ldr r1, [r1]
    tst r1, #0x800
    bne underflow_trap
    and r0, r0, #0x80000000
    cmn ip, #24
    beq underflow_shift_24
    bmi underflow_to_zero
    add r1, ip, #23
    mov r2, r2, lsl r1
    rsb ip, ip, #0
    mov r3, r3, lsr ip
    orr r0, r0, r3
    rsb lr, lr, #0
    mul r1, lr, r3
    cmp r1, r2
    addeq sp, sp, #8
    ldmeqfd sp!, {lr}
    bxeq lr
    add r1, r1, lr
    cmp r1, r2
    beq exact_plus_one
    addmi r0, r0, #1
    subpl r1, r1, lr
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
    bne round_toward_minus_inf_or_zero
    tst r3, #0x400000
    bne round_toward_plus_inf
    add r1, lr, r1, lsl #1
    cmp r1, r2, lsl #1
    and r3, r0, #1
    addmi r0, r0, #1
    addeq r0, r0, r3
    add sp, sp, #8
    ldmfd sp!, {lr}
    bx lr
exact_plus_one:
    add r0, r0, #1
    add sp, sp, #8
    ldmfd sp!, {lr}
    bx lr
underflow_shift_24:
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
    bne underflow_to_zero
    tst r3, #0x400000
    bne underflow_to_zero
    cmn r2, lr
    addne r0, r0, #1
    add sp, sp, #8
    ldmfd sp!, {lr}
    bx lr
b_nan:
    mov r0, r1
    b nan_operand
divide_by_zero:
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl func_02129748
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x200
    bne divide_by_zero_trap
    orr r3, r3, #2
    str r3, [r2]
return_inf:
    mov r0, #0xff000000
    orr r0, lr, r0, lsr #1
    add sp, sp, #8
    ldmfd sp!, {lr}
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
    b b_nan
nan_operand:
    mov r3, r0
    mvn r0, #0x80000000
    tst r3, #0x80000000
    addne sp, sp, #8
    ldmnefd sp!, {lr}
    bxne lr
    stmfd sp!, {r0, r1, r3, ip, lr}
    bl func_02129748
    mov r2, r0
    ldmfd sp!, {r0, r1, r3, ip, lr}
    ldr r3, [r2]
    tst r3, #0x100
    bne invalid_trap
    orr r3, r3, #1
    str r3, [r2]
    add sp, sp, #8
    ldmfd sp!, {lr}
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
    ldmfd sp!, {lr}
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
    tst r0, #0x80000000
    mov r0, #0xff000000
    movne r0, r0, asr #1
    moveq r0, r0, lsr #1
    tst r3, #0x800000
    bne overflow_toward_minus_inf
    tst r3, #0x400000
    subne r0, r0, r0, lsr #31
    add sp, sp, #8
    ldmfd sp!, {lr}
    bx lr
overflow_toward_minus_inf:
    sub r0, r0, #1
    tst r3, #0x400000
    addeq r0, r0, r0, lsr #31
    add sp, sp, #8
    ldmfd sp!, {lr}
    bx lr
underflow_to_zero:
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
    ldmeqfd sp!, {lr}
    bxeq lr
    cmp r0, #0
    addeq r0, r0, #1
    add sp, sp, #8
    ldmfd sp!, {lr}
    bx lr
tiny_toward_minus_inf_or_zero:
    tst r3, #0x400000
    addeq r0, r0, r0, lsr #31
    add sp, sp, #8
    ldmfd sp!, {lr}
    bx lr
return_zero:
    mov r0, lr
    add sp, sp, #8
    ldmfd sp!, {lr}
    bx lr
overflow_trap_after_round:
    add sp, sp, #8
    sub r0, r0, #0x60000000
    mov r1, #0x10000004
    bl func_02121ef4
    ldmfd sp!, {lr}
    bx lr
overflow_trap:
    add sp, sp, #8
    sub ip, ip, #0xc0
    add r0, r0, ip, lsl #23
    mov r1, #0x10000004
    bl func_02121ef4
    ldmfd sp!, {lr}
    bx lr
underflow_trap:
    add sp, sp, #8
    add ip, ip, #0xc0
    add r0, r0, ip, lsl #23
    mov r1, #0x20000004
    bl func_02121ef4
    ldmfd sp!, {lr}
    bx lr
divide_by_zero_trap:
    add sp, sp, #8
    ldr r2, =0x08000004
    bl func_02121e0c
    ldmfd sp!, {lr}
    bx lr
invalid_trap:
    ldmfd sp!, {r0, r1}
    ldr r2, =0x04000004
    bl func_02121e0c
    ldmfd sp!, {lr}
    bx lr
inexact_trap:
    add sp, sp, #8
    mov r1, #0x40000004
    bl func_02121ef4
    ldmfd sp!, {lr}
    bx lr
}
