#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Sorts gAiQueue by free cargo slots, most first. */
void AiQueue_SortBlackBoats(void);
void AiBlackBoat_Act(void); /* run on each queued Black Boat */

/* Queues the current player's Black Boats that have not acted yet. The
 * scan skips the player's first unit slot and covers the next 50. */
void AiAction_QueueBlackBoats(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        unit->type == UNIT_BLACK_BOAT) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  AiQueue_SortBlackBoats();
  gAiQueueRoutine = AiBlackBoat_Act;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
