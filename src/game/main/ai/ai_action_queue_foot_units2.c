#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u16 data_ov000_022f5c2c[]; /* per player slot: first slot in units */

/* Sorts gAiQueue by hp, largest first; a unit with an AI target counts
 * as half a point more. */
void AiQueue_SortByHp(void);
/* Foot unit action, run on each queued unit. */
void AiFoot_MoveAndAttack(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Queues the current player's infantry and mechs that have not acted,
 * whatever their AI order, highest hp first. Only slots 1-50 of the
 * player are checked. Same queue as AiAction_QueueFootUnits with a
 * different action routine. */
void AiAction_QueueFootUnits2(void)
{
    int i;
    MapUnit *unit;

    gAiQueueCursor = gAiQueue;
    for (i = data_ov000_022f5c2c[STATE->currentPlayer] + 1;
         i < data_ov000_022f5c2c[STATE->currentPlayer] + 51; i++) {
        unit = &STATE->units[i];
        if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0
            && Ov000_gUnitStats[unit->type].unk_20 == UNIT_AI_GROUP_FOOT) {
            *gAiQueueCursor++ = i;
        }
    }
    *gAiQueueCursor = 0;
    gAiQueueCursor = gAiQueue;
    AiQueue_SortByHp();
    gAiQueueRoutine = AiFoot_MoveAndAttack;
    *gAiTurnState = AI_STATE_RUN_QUEUE;
}
