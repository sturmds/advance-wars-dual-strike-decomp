#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* UnitStats.aiClass value that no unit type has in retail. */
#define UNIT_AI_CLASS_3 3

void AiClass3_Act(void); /* run on each queued unit */

/* Queues the current player's units that have not acted yet and whose
 * UnitStats.aiClass is 3. No unit type has that value, so in retail the
 * queue stays empty. The scan skips the player's first unit slot and
 * covers the next 50. */
void AiAction_QueueClass3Units(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == UNIT_AI_CLASS_3) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiClass3_Act;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
