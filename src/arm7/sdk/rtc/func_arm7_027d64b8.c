#include "sdk/rtc.h"

/* Receives `size` bytes into `data`, each low bit first: the data line is
 * turned to input, and each bit is sampled with the clock low. Bytes are
 * stored with halfword read-modify-writes only. */
#ifdef __MWERKS__
asm void func_arm7_027d64b8(void *data, int size)
{
    mov r12, #0x4000000
    add r12, r12, #0x138
byte:
    stmfd sp!, {r0, r1}
    ldrh r0, [r12, #0x0]
    bic r0, r0, #0x77
    orr r0, r0, #0x64
    mov r2, #0x0
    mov r1, #0x0
bit:
    bic r0, r0, #0x3
    orr r0, r0, #0x0
    strh r0, [r12, #0x0]
    mov r3, #0x9
wait1:
    subs r3, r3, #0x1
    bne wait1
    ldrh r0, [r12, #0x0]
    and r3, r0, #0x1
    cmp r3, #0x1
    moveq r3, #0x80
    movne r3, #0x0
    orr r2, r3, r2, lsr #0x1
    bic r0, r0, #0x2
    orr r0, r0, #0x2
    strh r0, [r12, #0x0]
    mov r3, #0x9
wait2:
    subs r3, r3, #0x1
    bne wait2
    add r1, r1, #0x1
    cmp r1, #0x8
    bne bit
    ldmfd sp!, {r0, r1}
    tst r0, #0x1
    beq even
    ldrh r3, [r0, #-0x1]
    bic r3, r3, #0xff00
    mov r2, r2, lsl #0x8
    orr r3, r2, r3
    strh r3, [r0, #-0x1]
    b next
even:
    ldrh r3, [r0, #0x0]
    bic r3, r3, #0xff
    orr r3, r3, r2
    strh r3, [r0, #0x0]
next:
    add r0, r0, #0x1
    subs r1, r1, #0x1
    bne byte
    bx lr
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* Counts count down to 0 like the assembly's wait loop. The counter is
   volatile so that the compiler keeps the loop. */
static void Delay(int count) {
  volatile int n = count;

  while (--n != 0) {
  }
}

/* The DS is little-endian, and StoreByte counts on it. */
static void CheckLittleEndian(const char* name) {
  const u16 one = 1;

  if (*(const u8*)&one != 1) {
    fprintf(stderr, "%s: StoreByte assumes a little-endian CPU\n", name);
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

/* The assembly decrements size before it tests it, so a size of 0 or less
   would receive about 4 billion bytes; such a size stops the program. */
void func_arm7_027d64b8(void* data, int size) {
  u8* to = data;
  u16 value;
  u32 byte;
  int bit;

  CheckLittleEndian("func_arm7_027d64b8");
  if (size <= 0) {
    fprintf(stderr, "func_arm7_027d64b8: size %d is not positive\n", size);
    abort();
  }
  do {
    value =
        (u16)((REG_RTC & ~(CLOCK_PINS_OUT | CLOCK_PINS)) | CLOCK_PIN_CLOCK_OUT |
              CLOCK_PIN_SELECT_OUT | CLOCK_PIN_SELECT);
    byte = 0;
    for (bit = 0; bit < 8; bit++) {
      value &= (u16) ~(CLOCK_PIN_DATA | CLOCK_PIN_CLOCK);
      REG_RTC = value;
      Delay(9);
      value = REG_RTC;
      byte = ((value & CLOCK_PIN_DATA) ? 0x80 : 0) | byte >> 1;
      value |= CLOCK_PIN_CLOCK;
      REG_RTC = value;
      Delay(9);
    }
    StoreByte(to++, (u8)byte);
  } while (--size != 0);
}
#endif
