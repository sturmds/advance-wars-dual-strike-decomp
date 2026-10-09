#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* UnitStats.moveGroup and UnitStats.fuel of each unit type, one row of
 * sizeof(UnitStats) bytes per type. Needed to match: the original reaches
 * them through their own symbols, not through Ov000_gUnitStats. */
extern u8 data_ov000_022f4fd7[][sizeof(UnitStats)];
extern u8 data_ov000_022f4fce[][sizeof(UnitStats)];

/* Whether the acting unit should go for supplies or repairs: it is under
 * 91 hp, or it is an air or sea unit with under half its fuel. */
BOOL AiUnit_NeedsService(void)
{
    if (STATE->actingUnit->hp <= 90) {
        return TRUE;
    }
    if (!(data_ov000_022f4fd7[STATE->actingUnit->type][0] & 7)
        && STATE->actingUnit->fuel < data_ov000_022f4fce[STATE->actingUnit->type][0] >> 1) {
        return TRUE;
    }
    return FALSE;
}
