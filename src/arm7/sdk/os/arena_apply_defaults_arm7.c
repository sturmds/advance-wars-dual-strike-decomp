#include "types.h"
#include "sdk/os_arm7.h"

extern BOOL data_arm7_03807a50;    /* set once the arenas are set up */

void func_arm7_037fc6fc(int id, int lo);    /* OS_SetArenaLo; lo is an address */
void func_arm7_037fc710(int id, int hi);    /* OS_SetArenaHi; hi is an address */
void *Arena_DefaultHighEnd_Arm7(ArenaRegionArm7 id);
void *Arena_DefaultLowEnd_Arm7(ArenaRegionArm7 id);

/* ARM7 copy of OS_InitArena. Gives each ARM7 arena its default bounds, once. */
void Arena_ApplyDefaults_Arm7(void)
{
    if (data_arm7_03807a50) {
        return;
    }
    data_arm7_03807a50 = TRUE;

    func_arm7_037fc710(OS_ARENA_MAIN_SUBPRIV, (int)Arena_DefaultHighEnd_Arm7(OS_ARENA_MAIN_SUBPRIV));
    func_arm7_037fc6fc(OS_ARENA_MAIN_SUBPRIV, (int)Arena_DefaultLowEnd_Arm7(OS_ARENA_MAIN_SUBPRIV));

    func_arm7_037fc710(OS_ARENA_WRAM_SUB, (int)Arena_DefaultHighEnd_Arm7(OS_ARENA_WRAM_SUB));
    func_arm7_037fc6fc(OS_ARENA_WRAM_SUB, (int)Arena_DefaultLowEnd_Arm7(OS_ARENA_WRAM_SUB));

    func_arm7_037fc710(OS_ARENA_WRAM_SUBPRIV, (int)Arena_DefaultHighEnd_Arm7(OS_ARENA_WRAM_SUBPRIV));
    func_arm7_037fc6fc(OS_ARENA_WRAM_SUBPRIV, (int)Arena_DefaultLowEnd_Arm7(OS_ARENA_WRAM_SUBPRIV));
}
