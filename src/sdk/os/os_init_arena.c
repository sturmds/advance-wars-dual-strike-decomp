#include "types.h"
#include "sdk/os.h"

extern BOOL data_022a9b74;    /* set once the arenas are set up */

void OS_SetArenaLo(OSArenaId id, void *lo);    /* sets an arena's low end */
void func_0210e270(OSArenaId id, void *hi);    /* sets an arena's high end */
void *func_0210e36c(OSArenaId id);             /* default high end of an arena */
/* Returns an arena's default low end, despite the name (SDK OS_GetInitArenaLo). */
void *OS_GetInitArenaHi(OSArenaId id);

/* Gives each arena its default bounds. The arena past 4 MB starts empty. */
void OS_InitArena(void)
{
    if (data_022a9b74) {
        return;
    }
    data_022a9b74 = TRUE;

    func_0210e270(OS_ARENA_MAIN, func_0210e36c(OS_ARENA_MAIN));
    OS_SetArenaLo(OS_ARENA_MAIN, OS_GetInitArenaHi(OS_ARENA_MAIN));

    OS_SetArenaLo(OS_ARENA_MAINEX, NULL);
    func_0210e270(OS_ARENA_MAINEX, NULL);

    func_0210e270(OS_ARENA_ITCM, func_0210e36c(OS_ARENA_ITCM));
    OS_SetArenaLo(OS_ARENA_ITCM, OS_GetInitArenaHi(OS_ARENA_ITCM));

    func_0210e270(OS_ARENA_DTCM, func_0210e36c(OS_ARENA_DTCM));
    OS_SetArenaLo(OS_ARENA_DTCM, OS_GetInitArenaHi(OS_ARENA_DTCM));

    func_0210e270(OS_ARENA_SHARED, func_0210e36c(OS_ARENA_SHARED));
    OS_SetArenaLo(OS_ARENA_SHARED, OS_GetInitArenaHi(OS_ARENA_SHARED));

    func_0210e270(OS_ARENA_WRAM_MAIN, func_0210e36c(OS_ARENA_WRAM_MAIN));
    OS_SetArenaLo(OS_ARENA_WRAM_MAIN, OS_GetInitArenaHi(OS_ARENA_WRAM_MAIN));
}
