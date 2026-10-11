#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Only an Oozium is limited: it may end its move on an empty tile or on a
 * unit of its own team. Every other unit type gets TRUE. */
BOOL ActingUnit_CanEndMoveOnDest(void)
{
    u8 slot;

    if (STATE->actingUnit->type != UNIT_OOZIUM) {
        return TRUE;
    }
    slot = STATE->mapTiles->unitAt[STATE->actingUnitDestX + STATE->mapTiles->rowStart[STATE->actingUnitDestY]];
    if (slot == 0 || STATE->players[MAP_UNIT_OWNER(STATE, STATE->actingUnit)].team == STATE->players[MAP_UNIT_OWNER(STATE, &STATE->units[slot])].team) {
        return TRUE;
    }
    return FALSE;
}
