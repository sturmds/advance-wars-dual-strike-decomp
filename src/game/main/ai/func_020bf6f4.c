#include "types.h"
#include "game/Unk_02080440.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

BOOL func_020bf954(void);
BOOL func_020bf2d0(void);
void RangeMap_Fill(int value);
int AiFoot_ListReachableSilos(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* After the two checks of func_020bf954 and func_020bf2d0 pass, clears the
 * range map, marks the acting unit's destination tile as reachable (0) and
 * returns whether no silo is reachable from there. Returns 1 as soon as
 * either check fails. The meaning of the checks is not known yet. */
s8 func_020bf6f4(void)
{
    if (!func_020bf954()) {
        return 1;
    }
    if (!func_020bf2d0()) {
        return 1;
    }
    RangeMap_Fill(-1);
    STATE->rangeMap[STATE->actingUnitDestY][STATE->actingUnitDestX] = 0;
    return AiFoot_ListReachableSilos() == 0;
}
