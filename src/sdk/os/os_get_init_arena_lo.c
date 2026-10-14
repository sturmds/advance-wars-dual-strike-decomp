#include "types.h"
#include "sdk/os.h"

BOOL data_022a9b78;
BOOL data_022a9b74; /* set once the arenas are set up */
u32 func_0210dbec(void);

/*
 * Where the linker placed the end of the code and data: the first address
 * past main and every overlay (0x0237a5a0 in retail, the end of ov001), and
 * the ends of ITCM and DTCM. tools/configure.py defines them in the linker
 * script (ARM9_LAYOUT_SYMBOLS), so they move when a module grows.
 */
extern char SDK_MAIN_ARENA_LO[];
extern char SDK_SECTION_ARENA_ITCM_START[];
extern char SDK_SECTION_ARENA_DTCM_START[];

/* Initial low end of each arena; 0 for an id with no arena. */
u32 OS_GetInitArenaLo(int id) {
  switch (id) {
    case 0:
      return (u32)SDK_MAIN_ARENA_LO;
    case 2:
      if (data_022a9b78 == 0 || (func_0210dbec() & 3) == 1) {
        return 0;
      }
      return 0x02400000;
    case 3:
      return (u32)SDK_SECTION_ARENA_ITCM_START;
    case 4:
      return (u32)SDK_SECTION_ARENA_DTCM_START;
    case 5:
      return 0x027ff000;
    case 6:
      return 0x037f8000;
    default:
      return 0;
  }
}
