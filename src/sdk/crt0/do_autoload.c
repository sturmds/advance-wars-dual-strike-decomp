#include "types.h"

extern u32 BuildInfo[];     /* module parameters the linker fills in */
void AutoloadCallback(int arg);

#ifdef __MWERKS__
/*
 * Copies the autoload blocks (ITCM, DTCM and the like) to where they run.
 * BuildInfo[0] and [1] bound a list of three-word entries: destination,
 * size to copy, size to clear after it. The data for all blocks follows one
 * another from BuildInfo[2]. Ends by jumping to AutoloadCallback.
 */
asm void do_autoload(void)
{
    ldr r0, =BuildInfo
    ldr r1, [r0]
    ldr r2, [r0, #4]
    ldr r3, [r0, #8]
next_block:
    cmp r1, r2
    beq done
    ldr r4, [r1], #4
    ldr r5, [r1], #4
    add r6, r4, r5
copy:
    cmp r4, r6
    ldrmi r7, [r3], #4
    strmi r7, [r4], #4
    bmi copy
    ldr r5, [r1], #4
    add r6, r4, r5
    mov r7, #0
clear:
    cmp r4, r6
    strcc r7, [r4], #4
    bcc clear
    beq next_block
done:
    b AutoloadCallback
}
#else
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

/* The table words are addresses on the DS. A copy size that is not a
   multiple of 4 copies one more word; a clear size that is not one ends the
   list after that block, as the assembly's last compare is then not equal.
   The word loads and stores would round misaligned pointers down, so one
   stops the program. */
void do_autoload(void) {
  u32* entry = (u32*)(uintptr_t)BuildInfo[0];
  u32* entries_end = (u32*)(uintptr_t)BuildInfo[1];
  const u32* src = (const u32*)(uintptr_t)BuildInfo[2];

  while (entry != entries_end) {
    u32* dst = (u32*)(uintptr_t)entry[0];
    u8* end = (u8*)dst + entry[1];

    if ((uintptr_t)dst % 4 != 0 || (uintptr_t)src % 4 != 0) {
      fprintf(stderr, "do_autoload: %p or %p is not word-aligned\n", (void*)dst,
              (const void*)src);
      abort();
    }
    while ((u8*)dst < end) {
      *dst++ = *src++;
    }
    end = (u8*)dst + entry[2];
    while ((u8*)dst < end) {
      *dst++ = 0;
    }
    entry += 3;
    if ((u8*)dst != end) {
      break;
    }
  }
  /* r0 still holds BuildInfo's address when the assembly jumps. */
  AutoloadCallback((int)(intptr_t)BuildInfo);
}
#endif
