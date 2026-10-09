#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440 *data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

void func_0209fadc(void); /* run on each queued Oozium */

/* Queues the current player's Oozium that have not acted yet. The scan
 * skips the player's first unit slot and covers the next 50. */
void AiAction_QueueOoziums(void)
{
    int i;
    MapUnit *unit;

    gAiQueueCursor = gAiQueue;
    for (i = data_ov000_022f5c2c[STATE->currentPlayer] + 1; i < data_ov000_022f5c2c[STATE->currentPlayer] + 51; i++) {
        unit = &STATE->units[i];
        if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 && unit->type == UNIT_OOZIUM) {
            *gAiQueueCursor++ = i;
        }
    }
    *gAiQueueCursor = 0;
    gAiQueueCursor = gAiQueue;
    gAiQueueRoutine = func_0209fadc;
    gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
