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
/* Direct combat unit action, a variant of AiUnit_ActByOrder that only this
 * step uses; switches on the unit's AI order. */
void func_0209ffe4(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Queues every direct combat unit of the current player that has not
 * acted, whatever its AI order. Only slots 1-50 of the player are
 * checked. Only the second step table (0x02161ae8, the Fog of War
 * list) uses this step, and it sorts by vision with the largest first. */
void AiAction_QueueAllDirectUnitsFog(void)
{
    int i;
    MapUnit *unit;

    gAiQueueCursor = gAiQueue;
    for (i = data_ov000_022f5c2c[STATE->currentPlayer] + 1;
         i < data_ov000_022f5c2c[STATE->currentPlayer] + 51; i++) {
        unit = &STATE->units[i];
        if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0
            && Ov000_gUnitStats[unit->type].unk_20 == AI_CLASS_DIRECT) {
            *gAiQueueCursor++ = i;
        }
    }
    *gAiQueueCursor = 0;
    gAiQueueCursor = gAiQueue;
    /* rain also brings fog */
    if (STATE->unk_1c4->fog != 0 || STATE->unk_1c4->weather == WEATHER_RAIN) {
        func_0208f724(TRUE);
    } else {
        func_0208f850(FALSE);
    }
    gAiQueueRoutine = func_0209ffe4;
    *gAiTurnState = AI_STATE_RUN_QUEUE;
}
