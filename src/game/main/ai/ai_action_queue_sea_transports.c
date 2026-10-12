#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiQueue_SortSeaTransports(void);
void AiSeaTransport_Act(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Slots 1-50 of the current player's 64; slot 0 would read as the end of
 * the queue for player 1. */
#define FIRST_UNIT (Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1)

/* Queues the current player's Black Boats and Landers that have not
 * waited yet, hands the queue to AiQueue_SortSeaTransports, and runs
 * AiSeaTransport_Act on each. */
void AiAction_QueueSeaTransports(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_SEA_TRANSPORT) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  AiQueue_SortSeaTransports();
  gAiQueueRoutine = AiSeaTransport_Act;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
