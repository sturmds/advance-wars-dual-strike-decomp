#include "types.h"
#include "sdk/os.h"

/*
 * Link-time constants of the SDK build. The ROM keeps testing them at run
 * time, so they must stay symbols: SYS_STACKSIZE 0, IRQ_STACKSIZE 0x400,
 * SECTION_ARENA_DTCM_START 0x027c12e0. tools/configure.py defines them in
 * the linker script (ARM9_ABSOLUTE_SYMBOLS).
 */
extern char SDK_SYS_STACKSIZE[];
extern char SDK_IRQ_STACKSIZE[];
extern char SDK_SECTION_ARENA_DTCM_START[];

extern u8 data_027c0000[]; /* DTCM base */
#define HW_DTCM_IRQ_STACK_END ((u32)data_027c0000 + 0x3f80)

/* Default upper ends of the fixed arenas. */
#define MAIN_ARENA_HI 0x023c0000
#define MAINEX_ARENA_HI 0x02700000
#define ITCM_ARENA_HI 0x02000000
#define SHARED_ARENA_HI 0x027ff800
#define WRAM_MAIN_ARENA_HI 0x037f8000

u32 func_0210dbec(void); /* console type */

void* OS_GetInitArenaHi(OSArenaId id) {
  u32 irqStackLo;
  u32 sysStackLo;

  switch (id) {
    case OS_ARENA_MAIN:
      return (void*)MAIN_ARENA_HI;
    case OS_ARENA_MAINEX:
      if (!data_022a9b78 || (func_0210dbec() & 3) == 1) { /* 4 MB console */
        return NULL;
      }
      return (void*)MAINEX_ARENA_HI;
    case OS_ARENA_ITCM:
      return (void*)ITCM_ARENA_HI;
    case OS_ARENA_DTCM:
      irqStackLo = HW_DTCM_IRQ_STACK_END - (u32)SDK_IRQ_STACKSIZE;
      if ((u32)SDK_SYS_STACKSIZE == 0) {
        sysStackLo = (u32)data_027c0000;
        if (sysStackLo < (u32)SDK_SECTION_ARENA_DTCM_START) {
          sysStackLo = (u32)SDK_SECTION_ARENA_DTCM_START;
        }
      } else if ((s32)SDK_SYS_STACKSIZE < 0) {
        sysStackLo = (u32)data_027c0000 - (u32)SDK_SYS_STACKSIZE;
      } else {
        sysStackLo = irqStackLo - (u32)SDK_SYS_STACKSIZE;
      }
      return (void*)sysStackLo;
    case OS_ARENA_SHARED:
      return (void*)SHARED_ARENA_HI;
    case OS_ARENA_WRAM_MAIN:
      return (void*)WRAM_MAIN_ARENA_HI;
    default:
      return NULL;
  }
}
