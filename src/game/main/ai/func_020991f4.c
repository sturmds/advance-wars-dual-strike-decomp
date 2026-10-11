#include "types.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

int func_020995a4(void);

/* When func_020995a4 says so: TRUE if the current player has a unit on the
 * map (not carried) with less than 91 hp. Nothing calls it. */
BOOL func_020991f4(void)
{
    int i;
    MapUnit *unit;

    if (func_020995a4() == 1) {
        for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1; i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
            unit = &STATE->units[i];
            if (unit->type != UNIT_NONE && !(unit->flags & MAP_UNIT_CARRIED) && unit->hp < 91) {
                return TRUE;
            }
        }
    }
    return FALSE;
}
