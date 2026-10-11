#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

/* TRUE when the unit has at most half of its type's full fuel. Types with
 * no fuel never count. */
static inline BOOL Unit_IsFuelAtMostHalf(MapUnit *unit)
{
    int maxFuel = Ov000_gUnitStats[unit->type].fuel;
    BOOL low = FALSE;

    if (maxFuel != 0 && unit->fuel <= maxFuel >> 1) {
        low = TRUE;
    }
    return low;
}

/* The original turns the result into a BOOL twice more before comparing it
 * with TRUE; these two wrappers keep both steps. */
static inline BOOL Unit_IsFuelLow(MapUnit *unit)
{
    BOOL low = FALSE;

    if (Unit_IsFuelAtMostHalf(unit)) {
        low = TRUE;
    }
    return low;
}

static inline BOOL Unit_NeedsFuel(MapUnit *unit)
{
    return Unit_IsFuelLow(unit) ? TRUE : FALSE;
}

/* TRUE when the acting unit is a Stealth with at most half its fuel. */
BOOL func_ov001_02351268(void)
{
    /* volatile read of the index first: the original loads it before the
     * state pointer */
    int index = *(volatile int *)&data_027c0284;
    MapUnit *unit = data_027c027c->unk_48[index]->actingUnit;

    if (unit->type != UNIT_STEALTH) {
        return FALSE;
    }
    return Unit_NeedsFuel(unit) == TRUE;
}
