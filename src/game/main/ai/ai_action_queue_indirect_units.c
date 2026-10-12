#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void AiQueue_SortByMaxRange(void);
void AiUnit_ActByOrderGroup(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Slots 1-50 of the current player's 64; slot 0 would read as the end of
 * the queue for player 1. */
#define FIRST_UNIT (Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1)

/* Queues the current player's indirect units (AI_CLASS_INDIRECT) that
 * have not waited yet, sorts them by range (AiQueue_SortByMaxRange), and
 * runs AiUnit_ActByOrderGroup on each. */
void AiAction_QueueIndirectUnits(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = FIRST_UNIT; i < FIRST_UNIT + 50; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_INDIRECT) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  AiQueue_SortByMaxRange();
  gAiQueueRoutine = AiUnit_ActByOrderGroup;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
