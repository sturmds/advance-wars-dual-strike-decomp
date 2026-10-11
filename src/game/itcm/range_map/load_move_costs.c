#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_stats.h"
#include "game/co_data.h"
#include "game/range_map.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern MoveSearch *data_027c0758;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Copies the move cost of each terrain id for a unit type into the move
 * search, from the movement chart of the CO that unk_1e points to. No CO
 * effects (coEffectsOn 0): CO 0's chart. */
void MoveSearch_LoadMoveCosts(int unitType)
{
    UnitStats *stats = &Ov000_gUnitStats[unitType];
    int row = stats->moveType << 5;
    int terrain;
    int coId;

    for (terrain = 0; terrain < 32; terrain++) {
        if (STATE->coEffectsOn == 0) {
            coId = 0;
        } else {
            coId = STATE->players[STATE->unk_1e].front.id & PLAYER_CO_ID_MASK;
        }
        data_027c0758->moveCost[terrain] =
            (((const s8 *)gCoTable[coId].blocks[STATE->players[STATE->unk_1e].front.powerState]
                  .movementCost[STATE->players[STATE->unk_1e].moveChart]) + row)[terrain];
    }
}
