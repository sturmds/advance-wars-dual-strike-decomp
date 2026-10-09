#include "types.h"
#include "game/map_unit.h"

/* Called by AiTurn_UpdateUnitModes (AI turn start) for units whose unk_0a_12 is 2:
 * the mode ends once the unit has more than 60 HP. */
void func_0208d3fc(MapUnit *unit)
{
    if (unit->hp >= 61) {
        unit->unk_0a_12 = 0;
    }
}
