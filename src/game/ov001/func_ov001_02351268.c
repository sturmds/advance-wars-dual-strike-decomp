#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
#define STATE (data_027c027c->unk_48[data_027c0284])

static inline BOOL Low(MapUnit *unit)
{
    int maxFuel = Ov000_gUnitStats[unit->type].fuel;
    BOOL low = FALSE;
    if (maxFuel != 0 && unit->fuel <= maxFuel >> 1) low = TRUE;
    return low;
}
static inline BOOL Chk(MapUnit *unit)
{
    BOOL r = FALSE;
    if (Low(unit)) r = TRUE;
    return r;
}
BOOL func_ov001_02351268(void)
{
    MapUnit *unit = STATE->actingUnit;
    if (unit->type != UNIT_STEALTH) return FALSE;
    return Chk(unit) == TRUE;
}
