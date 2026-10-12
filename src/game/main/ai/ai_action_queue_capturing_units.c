#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Run on each queued unit; continues the capture (romhacking.net doc 890). */
void AiFoot_ContinueCapture(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Queues the current player's infantry and mechs that have not acted
 * and are partway through a capture. Only slots 1-50 of the player are
 * checked. */
void AiAction_QueueCapturingUnits(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_FOOT &&
        unit->captureProgress != 0) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  gAiQueueRoutine = AiFoot_ContinueCapture;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
