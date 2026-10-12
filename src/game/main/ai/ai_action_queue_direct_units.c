#include "types.h"
#include "game/ai_unit_action.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"
#include "game/unit_stats.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

/* Sort gAiQueue by each unit's vision (AiQueue_SortByVision) or movement
 * (AiQueue_SortByMovement); 0 sorts smallest first. */
void AiQueue_SortByVision(BOOL descending);
void AiQueue_SortByMovement(BOOL descending);
/* Direct combat unit action, run on each queued unit. */
void AiUnit_ActByOrder(void);
/* TRUE on a missile silo; otherwise depends on the tile (Map_IsCapturableByActingUnit). */
BOOL Map_IsSiloOrCapturableTile(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Queues the current player's direct combat units that have not acted,
 * leaving out units with AI order 4 unless Map_IsSiloOrCapturableTile accepts their
 * tile (AiAction_QueueDirectUnitsOrder4 takes those later). Only slots
 * 1-50 of the player are checked. */
void AiAction_QueueDirectUnits(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_DIRECT &&
        (Map_IsSiloOrCapturableTile(unit->x, unit->y) || unit->aiOrder != 4)) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  /* rain also brings fog */
  if (STATE->unk_1c4->fog != 0 || STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiQueue_SortByVision(FALSE);
  } else {
    AiQueue_SortByMovement(FALSE);
  }
  gAiQueueRoutine = AiUnit_ActByOrder;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
