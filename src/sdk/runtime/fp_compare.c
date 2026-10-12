#include "types.h"

/*
 * Float and double comparison helpers of the Metrowerks ARM runtime,
 * hand-written assembly. The compiler calls _feq, _fneq, _fls, _fleq, _fgr
 * and _fgeq for float ==, !=, <, <=, > and >=, and the _d versions for
 * double; the other entries return a three-way result or condition flags.
 *
 * A float arrives as its raw bits in r0 and r1. A double arrives in r0:r1
 * and r2:r3, low word first. x << 1 above 0xff000000 means a float NaN;
 * 0x200000 + (hi << 1) carrying out means a double exponent of all ones,
 * so infinity or NaN. Signed magnitude is turned into two's complement
 * (clear the sign, negate) so that one signed compare orders both signs,
 * and +0 equals -0.
 *
 * A NaN operand is an invalid operation when it is a signaling NaN (quiet
 * bit clear), and for the ordered tests (<, <=, >, >=) when it is any NaN.
 * An invalid operation calls the trap handler if bit 8 of the floating-point
 * status word is set; otherwise it sets the sticky invalid flag (bit 0) and
 * returns the unordered result, which the entry leaves in r2 (float) or ip
 * (double).
 *
 * Each entry and each shared handler is its own function here because the
 * inline assembler cannot export a label inside a function or branch to one
 * in another function. Handlers end by falling through into the next one.
 */

u32 *__fp_status(void); /* returns the floating-point status word */
float __fp_trap_ff(float a, float b, u32 op);
s32 func_02121d58(double a, double b, u32 op);

asm void FCmp_Invalid(void);
asm void FCmp_InvalidFlagsNe(void);
asm void FCmp_InvalidFlagsHi(void);
asm void FCmp_InvalidFlagsEq(void);
asm void FCmp_NanA(void);
asm void FCmp_NanB(void);
asm void FCmp_NanAFlags(void);
asm void FCmp_NanBFlags(void);
asm void DCmp_Invalid(void);
asm void DCmp_InvalidFlagsNe(void);
asm void DCmp_InvalidFlagsHi(void);
asm void DCmp_InvalidFlagsEq(void);
asm void DCmp_NanA(void);
asm void DCmp_NanB(void);
asm void DCmp_Unordered(void);
asm void DCmp_NanAFlags(void);
asm void DCmp_NanBFlags(void);
asm void DCmp_UnorderedFlags(void);
asm void FCmp_CallTrap(void);
asm void DCmp_CallTrap(void);

/* Three-way float compare: 0 equal, 1 less, 2 greater, 3 unordered.
   Only a signaling NaN is invalid. */
asm void func_02127134(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    mov r2, #3
    bcc FCmp_NanA
    cmp r3, r1, lsl #1
    bcc FCmp_NanB
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    moveq r0, #0
    movlt r0, #1
    movgt r0, #2
    bx lr
}

/* Three-way float compare like func_02127134, but any NaN is invalid. */
asm void func_02127178(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmpcs r3, r1, lsl #1
    movcc r2, #3
    bcc FCmp_Invalid
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    moveq r0, #0
    movlt r0, #1
    movgt r0, #2
    bx lr
}

/* a != b for floats. Unordered counts as not equal. */
asm void _fneq(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    mov r2, #1
    bcc FCmp_NanA
    cmp r3, r1, lsl #1
    bcc FCmp_NanB
    orr r3, r0, r1
    movs r3, r3, lsl #1
    moveq r0, #0
    bxeq lr
    cmp r0, r1
    movne r0, #1
    moveq r0, #0
    bx lr
}

/* a == b for floats. */
asm void _feq(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    mov r2, #0
    bcc FCmp_NanA
    cmp r3, r1, lsl #1
    bcc FCmp_NanB
    orr r3, r0, r1
    movs r3, r3, lsl #1
    moveq r0, #1
    bxeq lr
    cmp r0, r1
    moveq r0, #1
    movne r0, #0
    bx lr
}

/* Float equality in the Z flag (set when equal). The flags of the raw
   compare are only meaningful for Z. */
asm void func_02127228(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    bcc FCmp_NanAFlags
    cmp r3, r1, lsl #1
    bcc FCmp_NanBFlags
    orr r3, r0, r1
    movs r3, r3, lsl #1
    bxeq lr
    cmp r0, r1
    bx lr
}

/* a < b for floats. */
asm void _fls(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmpcs r3, r1, lsl #1
    movcc r2, #0
    bcc FCmp_Invalid
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    movlt r0, #1
    movge r0, #0
    bx lr
}

/* a <= b for floats. */
asm void _fleq(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmpcs r3, r1, lsl #1
    movcc r2, #0
    bcc FCmp_Invalid
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    movle r0, #1
    movgt r0, #0
    bx lr
}

/* Ordered float compare in the flags: Z set when equal, C clear when
   a < b. Unordered leaves C set and Z clear, so lo and ls tests fail. */
asm void func_021272c8(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmpcs r3, r1, lsl #1
    bcc FCmp_InvalidFlagsHi
    orr r3, r0, r1
    movs r3, r3, lsl #1
    cmpeq r0, r0
    bxeq lr
    orrs r2, r0, r1
    bmi L_021272f8
    cmp r0, r1
    bx lr
L_021272f8:
    cmp r1, r0
    bx lr
}

/* a > b for floats. */
asm void _fgr(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmpcs r3, r1, lsl #1
    movcc r2, #0
    bcc FCmp_Invalid
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    movgt r0, #1
    movle r0, #0
    bx lr
}

/* a >= b for floats. */
asm void _fgeq(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmpcs r3, r1, lsl #1
    movcc r2, #0
    bcc FCmp_Invalid
    cmp r0, #0
    bicmi r0, r0, #0x80000000
    rsbmi r0, r0, #0
    cmp r1, #0
    bicmi r1, r1, #0x80000000
    rsbmi r1, r1, #0
    cmp r0, r1
    movge r0, #1
    movlt r0, #0
    bx lr
}

/* Ordered float compare in the flags like func_021272c8, but unordered
   leaves Z set and C clear, so hi and hs tests fail. */
asm void func_02127378(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    cmpcs r3, r1, lsl #1
    bcc FCmp_InvalidFlagsEq
    orr r3, r0, r1
    movs r3, r3, lsl #1
    cmpeq r0, r0
    bxeq lr
    orrs r2, r0, r1
    bmi L_021273a8
    cmp r0, r1
    bx lr
L_021273a8:
    cmp r1, r0
    bx lr
}

/* Returns 1 when a float operand is a NaN, else 0. */
asm void func_021273b0(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    mov r2, #1
    bcc FCmp_NanA
    cmp r3, r1, lsl #1
    bcc FCmp_NanB
    mov r0, #0
    bx lr
}

/* Returns 1 when neither float operand is a NaN, else 0. */
asm void func_021273d0(void)
{
    mov r3, #0xff000000
    cmp r3, r0, lsl #1
    mov r2, #0
    bcc FCmp_NanA
    cmp r3, r1, lsl #1
    bcc FCmp_NanB
    mov r0, #1
    bx lr
}

/* Invalid float compare: trap, or set the sticky flag and return r2. */
asm void FCmp_Invalid(void)
{
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x100
    bne FCmp_CallTrap
    orr r3, r3, #1
    str r3, [ip]
    mov r0, r2
    bx lr
}

/* Invalid float compare that returns flags: Z clear. */
asm void FCmp_InvalidFlagsNe(void)
{
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x100
    bne FCmp_CallTrap
    orr r3, r3, #1
    str r3, [ip]
    movs r2, #1
    bx lr
}

/* Invalid float compare that returns flags: C set, Z clear. */
asm void FCmp_InvalidFlagsHi(void)
{
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x100
    bne FCmp_CallTrap
    orr r3, r3, #1
    str r3, [ip]
    mov r2, #0xff000000
    adds r2, r2, #0x10000000
    bx lr
}

/* Invalid float compare that returns flags: Z set, C clear. */
asm void FCmp_InvalidFlagsEq(void)
{
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr r3, [ip]
    tst r3, #0x100
    bne FCmp_CallTrap
    orr r3, r3, #1
    str r3, [ip]
    mov r2, #0
    adds r2, r2, #0
    bx lr
}

/* a is a NaN (r3 holds 0xff000000). Signaling is invalid; otherwise
   return r2 unless b needs the same check. */
asm void FCmp_NanA(void)
{
    tst r0, #0x400000
    beq FCmp_Invalid
    cmp r3, r1, lsl #1
    movcs r0, r2
    bxcs lr
}

/* b is a NaN. Signaling is invalid; otherwise return r2. */
asm void FCmp_NanB(void)
{
    tst r1, #0x400000
    beq FCmp_Invalid
    mov r0, r2
    bx lr
}

/* FCmp_NanA for the flag-returning equality test. */
asm void FCmp_NanAFlags(void)
{
    tst r0, #0x400000
    beq FCmp_InvalidFlagsNe
    cmp r3, r1, lsl #1
    bcc FCmp_NanBFlags
    movs r2, #1
    bx lr
}

/* FCmp_NanB for the flag-returning equality test. */
asm void FCmp_NanBFlags(void)
{
    tst r1, #0x400000
    beq FCmp_InvalidFlagsNe
    movs r2, #1
    bx lr
}

/* Three-way double compare: 0 equal, 1 less, 2 greater, 3 unordered.
   Only a signaling NaN is invalid. */
asm void func_021274f4(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_02127550
    cmn ip, r3, lsl #1
    bcs L_0212756c
L_02127508:
    orrs ip, r3, r1
    bmi L_02127528
    cmp r1, r3
    cmpeq r0, r2
    moveq r0, #0
    movcc r0, #1
    movhi r0, #2
    bx lr
L_02127528:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #0
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    moveq r0, #0
    movcc r0, #1
    movhi r0, #2
    bx lr
L_02127550:
    movne ip, #3
    bne DCmp_NanA
    cmp r0, #0
    movhi ip, #3
    bhi DCmp_NanA
    cmn ip, r3, lsl #1
    bcc L_02127508
L_0212756c:
    movne ip, #3
    bne DCmp_NanB
    cmp r2, #0
    movhi ip, #3
    bhi DCmp_NanB
    b L_02127508
}

/* Three-way double compare like func_021274f4, but any NaN is invalid. */
asm void func_02127584(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_021275e0
    cmn ip, r3, lsl #1
    bcs L_021275fc
L_02127598:
    orrs ip, r3, r1
    bmi L_021275b8
    cmp r1, r3
    cmpeq r0, r2
    moveq r0, #0
    movcc r0, #1
    movhi r0, #2
    bx lr
L_021275b8:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #0
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    moveq r0, #0
    movcc r0, #1
    movhi r0, #2
    bx lr
L_021275e0:
    movne ip, #3
    bne DCmp_Invalid
    cmp r0, #0
    movhi ip, #3
    bhi DCmp_Invalid
    cmn ip, r3, lsl #1
    bcc L_02127598
L_021275fc:
    movne ip, #3
    bne DCmp_Invalid
    cmp r2, #0
    movhi ip, #3
    bhi DCmp_Invalid
    b L_02127598
}

/* a != b for doubles. Unordered counts as not equal. */
asm void _dneq(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_02127668
    cmn ip, r3, lsl #1
    bcs L_02127684
L_02127628:
    orrs ip, r3, r1
    bmi L_02127644
    cmp r1, r3
    cmpeq r0, r2
    movne r0, #1
    moveq r0, #0
    bx lr
L_02127644:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #0
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movne r0, #1
    moveq r0, #0
    bx lr
L_02127668:
    movne ip, #1
    bne DCmp_NanA
    cmp r0, #0
    movhi ip, #1
    bhi DCmp_NanA
    cmn ip, r3, lsl #1
    bcc L_02127628
L_02127684:
    movne ip, #1
    bne DCmp_NanB
    cmp r2, #0
    movhi ip, #1
    bhi DCmp_NanB
    b L_02127628
}

/* a == b for doubles. */
asm void _deq(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_021276f0
    cmn ip, r3, lsl #1
    bcs L_0212770c
L_021276b0:
    orrs ip, r3, r1
    bmi L_021276cc
    cmp r1, r3
    cmpeq r0, r2
    moveq r0, #1
    movne r0, #0
    bx lr
L_021276cc:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #1
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    moveq r0, #1
    movne r0, #0
    bx lr
L_021276f0:
    movne ip, #0
    bne DCmp_NanA
    cmp r0, #0
    movhi ip, #0
    bhi DCmp_NanA
    cmn ip, r3, lsl #1
    bcc L_021276b0
L_0212770c:
    movne ip, #0
    bne DCmp_NanB
    cmp r2, #0
    movhi ip, #0
    bhi DCmp_NanB
    b L_021276b0
}

/* Double equality in the Z flag (set when equal). */
asm void func_02127724(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_02127764
    cmn ip, r3, lsl #1
    bcs L_02127778
L_02127738:
    orrs ip, r3, r1
    bmi L_0212774c
    cmp r1, r3
    cmpeq r0, r2
    bx lr
L_0212774c:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    bx lr
L_02127764:
    bne DCmp_NanAFlags
    cmp r0, #0
    bhi DCmp_NanAFlags
    cmn ip, r3, lsl #1
    bcc L_02127738
L_02127778:
    bne DCmp_NanBFlags
    cmp r2, #0
    bhi DCmp_NanBFlags
    b L_02127738
}

/* a < b for doubles. */
asm void _dls(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_021277dc
    cmn ip, r3, lsl #1
    bcs L_021277f8
L_0212779c:
    orrs ip, r3, r1
    bmi L_021277b8
    cmp r1, r3
    cmpeq r0, r2
    movcc r0, #1
    movcs r0, #0
    bx lr
L_021277b8:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #0
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movcc r0, #1
    movcs r0, #0
    bx lr
L_021277dc:
    movne ip, #0
    bne DCmp_Invalid
    cmp r0, #0
    movhi ip, #0
    bhi DCmp_Invalid
    cmn ip, r3, lsl #1
    bcc L_0212779c
L_021277f8:
    movne ip, #0
    bne DCmp_Invalid
    cmp r2, #0
    movhi ip, #0
    bhi DCmp_Invalid
    b L_0212779c
}

/* a <= b for doubles. */
asm void _dleq(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_02127864
    cmn ip, r3, lsl #1
    bcs L_02127880
L_02127824:
    orrs ip, r3, r1
    bmi L_02127840
    cmp r1, r3
    cmpeq r0, r2
    movls r0, #1
    movhi r0, #0
    bx lr
L_02127840:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #1
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movls r0, #1
    movhi r0, #0
    bx lr
L_02127864:
    movne ip, #0
    bne DCmp_Invalid
    cmp r0, #0
    movhi ip, #0
    bhi DCmp_Invalid
    cmn ip, r3, lsl #1
    bcc L_02127824
L_02127880:
    movne ip, #0
    bne DCmp_Invalid
    cmp r2, #0
    movhi ip, #0
    bhi DCmp_Invalid
    b L_02127824
}

/* Ordered double compare in the flags: Z set when equal, C clear when
   a < b. Unordered leaves C set and Z clear. */
asm void func_02127898(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_021278dc
    cmn ip, r3, lsl #1
    bcs L_021278f0
L_021278ac:
    orrs ip, r3, r1
    bmi L_021278c0
    cmp r1, r3
    cmpeq r0, r2
    bx lr
L_021278c0:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    cmpeq r1, r1
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    bx lr
L_021278dc:
    bne DCmp_InvalidFlagsHi
    cmp r0, #0
    bhi DCmp_InvalidFlagsHi
    cmn ip, r3, lsl #1
    bcc L_021278ac
L_021278f0:
    bne DCmp_InvalidFlagsHi
    cmp r2, #0
    bhi DCmp_InvalidFlagsHi
    b L_021278ac
}

/* a > b for doubles. */
asm void _dgr(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_02127954
    cmn ip, r3, lsl #1
    bcs L_02127970
L_02127914:
    orrs ip, r3, r1
    bmi L_02127930
    cmp r1, r3
    cmpeq r0, r2
    movhi r0, #1
    movls r0, #0
    bx lr
L_02127930:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #0
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movhi r0, #1
    movls r0, #0
    bx lr
L_02127954:
    movne ip, #0
    bne DCmp_Invalid
    cmp r0, #0
    movhi ip, #0
    bhi DCmp_Invalid
    cmn ip, r3, lsl #1
    bcc L_02127914
L_02127970:
    movne ip, #0
    bne DCmp_Invalid
    cmp r2, #0
    movhi ip, #0
    bhi DCmp_Invalid
    b L_02127914
}

/* a >= b for doubles. */
asm void _dgeq(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_021279dc
    cmn ip, r3, lsl #1
    bcs L_021279f8
L_0212799c:
    orrs ip, r3, r1
    bmi L_021279b8
    cmp r1, r3
    cmpeq r0, r2
    movcs r0, #1
    movcc r0, #0
    bx lr
L_021279b8:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    moveq r0, #1
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    movcs r0, #1
    movcc r0, #0
    bx lr
L_021279dc:
    movne ip, #0
    bne DCmp_Invalid
    cmp r0, #0
    movhi ip, #0
    bhi DCmp_Invalid
    cmn ip, r3, lsl #1
    bcc L_0212799c
L_021279f8:
    movne ip, #0
    bne DCmp_Invalid
    cmp r2, #0
    movhi ip, #0
    bhi DCmp_Invalid
    b L_0212799c
}

/* Ordered double compare in the flags like func_02127898, but unordered
   leaves Z set and C clear. */
asm void func_02127a10(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_02127a54
    cmn ip, r3, lsl #1
    bcs L_02127a68
L_02127a24:
    orrs ip, r3, r1
    bmi L_02127a38
    cmp r1, r3
    cmpeq r0, r2
    bx lr
L_02127a38:
    orr ip, r0, ip, lsl #1
    orrs ip, ip, r2
    cmpeq r1, r1
    bxeq lr
    cmp r3, r1
    cmpeq r2, r0
    bx lr
L_02127a54:
    bne DCmp_InvalidFlagsEq
    cmp r0, #0
    bhi DCmp_InvalidFlagsEq
    cmn ip, r3, lsl #1
    bcc L_02127a24
L_02127a68:
    bne DCmp_InvalidFlagsEq
    cmp r2, #0
    bhi DCmp_InvalidFlagsEq
    b L_02127a24
}

/* Returns 1 when a double operand is a NaN, else 0. */
asm void func_02127a78(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_02127a94
    cmn ip, r3, lsl #1
    bcs L_02127ab0
L_02127a8c:
    mov r0, #0
    bx lr
L_02127a94:
    movne ip, #1
    bne DCmp_NanA
    cmp r0, #0
    movhi ip, #1
    bhi DCmp_NanA
    cmn ip, r3, lsl #1
    bcc L_02127a8c
L_02127ab0:
    movne ip, #1
    bne DCmp_NanB
    cmp r2, #0
    movhi ip, #1
    bhi DCmp_NanB
    b L_02127a8c
}

/* Returns 1 when neither double operand is a NaN, else 0. */
asm void func_02127ac8(void)
{
    mov ip, #0x200000
    cmn ip, r1, lsl #1
    bcs L_02127ae4
    cmn ip, r3, lsl #1
    bcs L_02127b00
L_02127adc:
    mov r0, #1
    bx lr
L_02127ae4:
    movne ip, #0
    bne DCmp_NanA
    cmp r0, #0
    movhi ip, #0
    bhi DCmp_NanA
    cmn ip, r3, lsl #1
    bcc L_02127adc
L_02127b00:
    movne ip, #0
    bne DCmp_NanB
    cmp r2, #0
    movhi ip, #0
    bhi DCmp_NanB
    b L_02127adc
}

/* Invalid double compare: trap, or set the sticky flag and return ip.
   The caller's ip and lr stay on the stack for DCmp_CallTrap. */
asm void DCmp_Invalid(void)
{
    stmfd sp!, {ip, lr}
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr lr, [ip]
    tst lr, #0x100
    bne DCmp_CallTrap
    orr lr, lr, #1
    str lr, [ip]
    ldmfd sp!, {r0}
    ldmfd sp!, {lr}
    bx lr
}

/* Invalid double compare that returns flags: Z clear. */
asm void DCmp_InvalidFlagsNe(void)
{
    stmfd sp!, {ip, lr}
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr lr, [ip]
    tst lr, #0x100
    bne DCmp_CallTrap
    orr lr, lr, #1
    str lr, [ip]
    add sp, sp, #4
    movs ip, #1
    ldmfd sp!, {lr}
    bx lr
}

/* Invalid double compare that returns flags: C set, Z clear. */
asm void DCmp_InvalidFlagsHi(void)
{
    stmfd sp!, {ip, lr}
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr lr, [ip]
    tst lr, #0x100
    bne DCmp_CallTrap
    orr lr, lr, #1
    str lr, [ip]
    add sp, sp, #4
    mov ip, #0xff000000
    adds ip, ip, #0x10000000
    ldmfd sp!, {lr}
    bx lr
}

/* Invalid double compare that returns flags: Z set, C clear. */
asm void DCmp_InvalidFlagsEq(void)
{
    stmfd sp!, {ip, lr}
    stmfd sp!, {r0, r1, r2, r3, lr}
    bl __fp_status
    mov ip, r0
    ldmfd sp!, {r0, r1, r2, r3, lr}
    ldr lr, [ip]
    tst lr, #0x100
    bne DCmp_CallTrap
    orr lr, lr, #1
    str lr, [ip]
    add sp, sp, #4
    mov ip, #0
    adds ip, ip, #0
    ldmfd sp!, {lr}
    bx lr
}

/* a is a NaN. Signaling is invalid; otherwise return ip unless b is
   also a NaN. */
asm void DCmp_NanA(void)
{
    tst r1, #0x80000
    beq DCmp_Invalid
    stmfd sp!, {ip}
    mov ip, #0x200000
    cmn ip, r3, lsl #1
    ldmfd sp!, {ip}
    bcc DCmp_Unordered
    bne DCmp_NanB
    cmp r2, #0
    beq DCmp_Unordered
}

/* b is a NaN. Signaling is invalid; otherwise return ip. */
asm void DCmp_NanB(void)
{
    tst r3, #0x80000
    beq DCmp_Invalid
}

/* Returns the unordered result left in ip. */
asm void DCmp_Unordered(void)
{
    mov r0, ip
    bx lr
}

/* DCmp_NanA for the flag-returning equality test. */
asm void DCmp_NanAFlags(void)
{
    tst r1, #0x80000
    beq DCmp_InvalidFlagsNe
    stmfd sp!, {ip}
    mov ip, #0x200000
    cmn ip, r3, lsl #1
    ldmfd sp!, {ip}
    bcc DCmp_UnorderedFlags
    bne DCmp_NanBFlags
    cmp r2, #0
    beq DCmp_UnorderedFlags
}

/* DCmp_NanB for the flag-returning equality test. */
asm void DCmp_NanBFlags(void)
{
    tst r3, #0x80000
    beq DCmp_InvalidFlagsNe
}

/* Unordered result of the flag-returning equality test: Z clear. */
asm void DCmp_UnorderedFlags(void)
{
    movs ip, #1
    bx lr
}

/* Calls the invalid-operation trap for floats and returns its result.
   The pc-relative load reads the first word of DCmp_CallTrap's pool
   (0x04000049). */
asm void FCmp_CallTrap(void)
{
    stmfd sp!, {lr}
    ldr r2, [pc, #32]
    bl __fp_trap_ff
    ldmfd sp!, {lr}
    bx lr
}

/* Calls the invalid-operation trap for doubles, with the operation word
   on the stack, and returns its result. In both operation words
   0x04000000 selects the invalid-operation handler; the low bytes 0x49
   and 0xc9 differ only in bit 7, which is likely the operand size. */
asm void DCmp_CallTrap(void)
{
    ldr ip, [pc, #20]
    stmfd sp!, {ip}
    bl func_02121d58
    add sp, sp, #8
    ldmfd sp!, {lr}
    bx lr
    dcd 0x04000049
    dcd 0x040000c9
}
