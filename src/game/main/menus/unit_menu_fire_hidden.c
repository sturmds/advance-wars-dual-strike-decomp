#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u8 data_022a6b34[];

void RangeMap_Fill(int value);
void RangeMap_FillAttackRange(int x, int y, MapUnit *unit);
BOOL FireTargets_Build(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Unit menu entry test for the first Fire entry (table at 0x02166050).
 * 0 shows the entry, 1 hides it. It shows when nothing is pending in
 * data_022a6b34, no other unit stands on the destination tile, the unit has
 * a weapon (only a direct one after moving), and FireTargets_Build finds
 * something to fire on in its attack range. */
s8 UnitMenu_FireHidden(void)
{
    if (data_022a6b34[0] + data_022a6b34[1] != 0) {
        return 1;
    }
    if (STATE->mapTiles->unitAt[STATE->actingUnitDestX + STATE->mapTiles->rowStart[STATE->actingUnitDestY]] != 0) {
        return 1;
    }
    if (STATE->actingUnitDestX != STATE->actingUnitStartX || STATE->actingUnitDestY != STATE->actingUnitStartY) {
        if (Ov000_gUnitStats[STATE->actingUnit->type].minRange != ATTACK_DIRECT) {
            return 1;
        }
    }
    if (Ov000_gUnitStats[STATE->actingUnit->type].minRange == ATTACK_NONE) {
        return 1;
    }
    RangeMap_Fill(-1);
    RangeMap_FillAttackRange(STATE->actingUnitDestX, STATE->actingUnitDestY, STATE->actingUnit);
    return FireTargets_Build() == 0;
}
