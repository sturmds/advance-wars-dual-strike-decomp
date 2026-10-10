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

    if (maxFuel != 0 && unit->fuel <= maxFuel >> 1) {
        low = TRUE;
    }
    return low;
}

BOOL func_ov001_02351174(void)
{
    int i;

    for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1; i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
        MapUnit *unit = &STATE->units[i];

        if (unit->type != 0 && unit->type == UNIT_STEALTH) {
            BOOL r = FALSE;

            if (Low(unit)) {
                r = TRUE;
            }
            if (r == TRUE) {
                return TRUE;
            }
        }
    }
    return FALSE;
}
