#include "types.h"

#ifdef __MWERKS__
asm void Mem_CopyBytes_Arm7(const void *src, void *dest, u32 size)
{
    cmp r2, #0
    bxeq lr

     
    tst r1, #1
    beq dest_even
    ldrh r12, [r1, #-1]
    and r12, r12, #0xff
    tst r0, #1
    ldrneh r3, [r0, #-1]
    movne r3, r3, lsr #8
    ldreqh r3, [r0]
    orr r3, r12, r3, lsl #8
    strh r3, [r1, #-1]
    add r0, r0, #1
    add r1, r1, #1
    subs r2, r2, #1
    bxeq lr

dest_even:
    eor r12, r1, r0
    tst r12, #1
    beq same_parity

     
    bic r0, r0, #1
    ldrh r12, [r0], #2
    mov r3, r12, lsr #8
    subs r2, r2, #2
    bcc shifted_tail
shifted_loop:
    ldrh r12, [r0], #2
    orr r12, r3, r12, lsl #8
    strh r12, [r1], #2
    mov r3, r12, lsr #16
    subs r2, r2, #2
    bcs shifted_loop
shifted_tail:
    tst r2, #1
    bxeq lr
    ldrh r12, [r1]
    and r12, r12, #0xff00
    orr r12, r12, r3
    strh r12, [r1]
    bx lr

same_parity:
    tst r12, #2
    beq same_word_alignment

     
    bics r3, r2, #1
    beq last_byte
    sub r2, r2, r3
    add r12, r3, r1
halfword_loop:
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    cmp r1, r12
    bcc halfword_loop
    b last_byte

same_word_alignment:
    cmp r2, #2
    bcc last_byte
    tst r1, #2
    beq word_aligned
    ldrh r3, [r0], #2
    strh r3, [r1], #2
    subs r2, r2, #2
    bxeq lr
word_aligned:
    bics r3, r2, #3
    beq last_halfword
    sub r2, r2, r3
    add r12, r3, r1
word_loop:
    ldr r3, [r0], #4
    str r3, [r1], #4
    cmp r1, r12
    bcc word_loop
last_halfword:
    tst r2, #2
    ldrneh r3, [r0], #2
    strneh r3, [r1], #2

last_byte:
     
    tst r2, #1
    bxeq lr
    ldrh r2, [r1]
    ldrh r0, [r0]
    and r2, r2, #0xff00
    and r0, r0, #0xff
    orr r0, r2, r0
    strh r0, [r1]
    bx lr
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* The DS is little-endian, and the byte helpers below count on it. */
static void CheckLittleEndian(const char* name) {
  const u16 one = 1;

  if (*(const u8*)&one != 1) {
    fprintf(stderr, "%s: the byte helpers assume a little-endian CPU\n", name);
    abort();
  }
}

/* Writes value into the byte at p by rewriting the halfword that holds it,
   as the assembly does: VRAM ignores byte stores. */
static void StoreByte(u8* p, u8 value) {
  u16* halfword = (u16*)((uintptr_t)p & ~(uintptr_t)1);
  u32 shift = ((uintptr_t)p & 1) * 8;

  *halfword = (u16)((*halfword & ~(0xff << shift)) | value << shift);
}

/* Reads the byte at p through the halfword that holds it. */
static u8 LoadByte(const u8* p) {
  const u16* halfword = (const u16*)((uintptr_t)p & ~(uintptr_t)1);

  return (u8)(*halfword >> ((uintptr_t)p & 1) * 8);
}

/* Copies size bytes from src to dest, with halfword accesses only. The
   assembly copies a halfword or a word at a time where it can, so when dest
   lies inside the source after src the result depends on those steps;
   overlapping regions stop the program. */
void Mem_CopyBytes_Arm7(const void* src, void* dest, u32 size) {
  const u8* from = src;
  u8* to = dest;
  u32 i;

  CheckLittleEndian("Mem_CopyBytes_Arm7");
  if (size != 0 && (uintptr_t)from < (uintptr_t)to + size &&
      (uintptr_t)to < (uintptr_t)from + size) {
    fprintf(stderr, "Mem_CopyBytes_Arm7: %p and %p overlap\n", src, dest);
    abort();
  }
  for (i = 0; i < size; i++) {
    StoreByte(to + i, LoadByte(from + i));
  }
}
#endif
