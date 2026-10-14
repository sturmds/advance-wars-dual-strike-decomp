#include "types.h"

/*
 * Finds out whether main RAM is 4 MB (retail) or 8 MB (debug hardware) and
 * stores 1 or 2 in the shared halfword at 0x027ffffa. With 4 MB, main RAM
 * repeats every 4 MB, so 0x023ffffa reads back what was written to
 * 0x027ffffa; the test writes 0 and 1 and compares both.
 */
#ifdef __MWERKS__
asm void DetectMainMemorySize(void)
{
    mov r0, #1
    mov r1, #0
    ldr r2, =0x027ffffa
    sub r3, r2, #0x400000
loop:
    strh r1, [r2]
    ldrh r12, [r3]
    cmp r1, r12
    movne r0, #2
    bne done
    add r1, r1, #1
    cmp r1, #2
    bne loop
done:
    strh r0, [r2]
    bx lr
}
#else
/* The values written to the top of 8 MB and looked for at the top of 4 MB. */
#define MEMORY_TEST_VALUES 2

void DetectMainMemorySize(void) {
  volatile u16* flag = (volatile u16*)0x027ffffa;
  volatile u16* alias = (volatile u16*)0x023ffffa;
  u16 size = 1;
  u16 i;

  for (i = 0; i < MEMORY_TEST_VALUES; i++) {
    *flag = i;
    if (*alias != i) {
      size = 2;
      break;
    }
  }
  *flag = size;
}
#endif
