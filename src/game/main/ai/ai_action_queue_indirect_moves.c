#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;
extern u16 data_ov000_022f5c2c[]; /* per player slot: 0, 0, 64, 128, 192 */

void func_0208f850(int arg);
void func_0209fc58(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Slots 1-50 of the current player's 64; slot 0 would read as the end of
 * the queue for player 1. */
#define FIRST_UNIT (data_ov000_022f5c2c[STATE->currentPlayer] + 1)

/* Queues the current player's indirect units (AI_CLASS_INDIRECT) that
 * have not waited yet, hands the queue to func_0208f850 (argument 1),
 * and runs func_0209fc58 on each. */
void AiAction_QueueIndirectMoves(void)
{
    int i;
    MapUnit *unit;

    gAiQueueCursor = gAiQueue;
    for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
        unit = &STATE->units[i];
        if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0
            && Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_INDIRECT) {
            *gAiQueueCursor++ = i;
        }
    }
    *gAiQueueCursor = 0;
    gAiQueueCursor = gAiQueue;
    func_0208f850(1);
    gAiQueueRoutine = func_0209fc58;
    gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
