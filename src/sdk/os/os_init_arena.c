#include "types.h"
#include "sdk/os.h"

extern BOOL data_022a9b74;    /* set once the arenas are set up */

void OS_SetArenaLo(OSArenaId id, void *lo);
void OS_SetArenaHi(OSArenaId id, void *hi);
void *OS_GetInitArenaHi(OSArenaId id);             /* default high end of an arena */
void *OS_GetInitArenaLo(OSArenaId id);

/* Gives each arena its default bounds. The arena past 4 MB starts empty. */
void OS_InitArena(void)
{
    if (data_022a9b74) {
        return;
    }
    data_022a9b74 = TRUE;

    OS_SetArenaHi(OS_ARENA_MAIN, OS_GetInitArenaHi(OS_ARENA_MAIN));
    OS_SetArenaLo(OS_ARENA_MAIN, OS_GetInitArenaLo(OS_ARENA_MAIN));

    OS_SetArenaLo(OS_ARENA_MAINEX, NULL);
    OS_SetArenaHi(OS_ARENA_MAINEX, NULL);

    OS_SetArenaHi(OS_ARENA_ITCM, OS_GetInitArenaHi(OS_ARENA_ITCM));
    OS_SetArenaLo(OS_ARENA_ITCM, OS_GetInitArenaLo(OS_ARENA_ITCM));

    OS_SetArenaHi(OS_ARENA_DTCM, OS_GetInitArenaHi(OS_ARENA_DTCM));
    OS_SetArenaLo(OS_ARENA_DTCM, OS_GetInitArenaLo(OS_ARENA_DTCM));

    OS_SetArenaHi(OS_ARENA_SHARED, OS_GetInitArenaHi(OS_ARENA_SHARED));
    OS_SetArenaLo(OS_ARENA_SHARED, OS_GetInitArenaLo(OS_ARENA_SHARED));

    OS_SetArenaHi(OS_ARENA_WRAM_MAIN, OS_GetInitArenaHi(OS_ARENA_WRAM_MAIN));
    OS_SetArenaLo(OS_ARENA_WRAM_MAIN, OS_GetInitArenaLo(OS_ARENA_WRAM_MAIN));
}
