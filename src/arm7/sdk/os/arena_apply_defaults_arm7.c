#include "types.h"
#include "sdk/os_arm7.h"

extern BOOL data_arm7_03807a50;    /* set once the arenas are set up */

/* ARM7 copy of OS_InitArena. Gives each ARM7 arena its default bounds, once. */
void Arena_ApplyDefaults_Arm7(void)
{
    if (data_arm7_03807a50) {
        return;
    }
    data_arm7_03807a50 = TRUE;

    Arena_SetHighEnd_Arm7(OS_ARENA_MAIN_SUBPRIV, Arena_DefaultHighEnd_Arm7(OS_ARENA_MAIN_SUBPRIV));
    Arena_SetLowEnd_Arm7(OS_ARENA_MAIN_SUBPRIV, Arena_DefaultLowEnd_Arm7(OS_ARENA_MAIN_SUBPRIV));

    Arena_SetHighEnd_Arm7(OS_ARENA_WRAM_SUB, Arena_DefaultHighEnd_Arm7(OS_ARENA_WRAM_SUB));
    Arena_SetLowEnd_Arm7(OS_ARENA_WRAM_SUB, Arena_DefaultLowEnd_Arm7(OS_ARENA_WRAM_SUB));

    Arena_SetHighEnd_Arm7(OS_ARENA_WRAM_SUBPRIV, Arena_DefaultHighEnd_Arm7(OS_ARENA_WRAM_SUBPRIV));
    Arena_SetLowEnd_Arm7(OS_ARENA_WRAM_SUBPRIV, Arena_DefaultLowEnd_Arm7(OS_ARENA_WRAM_SUBPRIV));
}
