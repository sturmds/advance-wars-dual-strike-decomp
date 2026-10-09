#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u16 data_ov000_022f5c2c[]; /* per player slot: first slot in units */

/* Sort gAiQueue by each unit's vision (func_0208f724) or movement
 * (func_0208f850); 0 sorts smallest first. */
void func_0208f724(BOOL descending);
void func_0208f850(BOOL descending);
/* Direct combat unit action, run on each queued unit. */
void AiUnit_ActByOrder(void);
/* TRUE on a missile silo; otherwise depends on the tile (func_020dedb0). */
BOOL func_0209e1ac(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Queues the current player's direct combat units that have not acted,
 * leaving out units with AI order 4 unless func_0209e1ac accepts their
 * tile (AiAction_QueueDirectUnitsOrder4 takes those later). Only slots
 * 1-50 of the player are checked. */
void AiAction_QueueDirectUnits(void)
{
    int i;
    MapUnit *unit;

    gAiQueueCursor = gAiQueue;
    for (i = data_ov000_022f5c2c[STATE->currentPlayer] + 1;
         i < data_ov000_022f5c2c[STATE->currentPlayer] + 51; i++) {
        unit = &STATE->units[i];
        if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0
            && Ov000_gUnitStats[unit->type].unk_20 == UNIT_AI_GROUP_DIRECT
            && (func_0209e1ac(unit->x, unit->y) || unit->aiOrder != 4)) {
            *gAiQueueCursor++ = i;
        }
    }
    *gAiQueueCursor = 0;
    gAiQueueCursor = gAiQueue;
    /* rain also brings fog */
    if (STATE->unk_1c4->fog != 0 || STATE->unk_1c4->weather == WEATHER_RAIN) {
        func_0208f724(FALSE);
    } else {
        func_0208f850(FALSE);
    }
    gAiQueueRoutine = AiUnit_ActByOrder;
    *gAiTurnState = AI_STATE_RUN_QUEUE;
}
