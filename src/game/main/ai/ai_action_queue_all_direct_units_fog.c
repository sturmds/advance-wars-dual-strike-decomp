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
/* Direct combat unit action, a variant of AiUnit_ActByOrder that only this
 * step uses; switches on the unit's AI order. */
void AiUnit_ActByOrderFog(void);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Queues every direct combat unit of the current player that has not
 * acted, whatever its AI order. Only slots 1-50 of the player are
 * checked. Only the second step table (0x02161ae8, the Fog of War
 * list) uses this step, and it sorts by vision with the largest first. */
void AiAction_QueueAllDirectUnitsFog(void) {
  int i;
  MapUnit* unit;

  gAiQueueCursor = gAiQueue;
  for (i = Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 1;
       i < Ov000_gPlayerFirstUnitSlot[STATE->currentPlayer] + 51; i++) {
    unit = &STATE->units[i];
    if (unit->type != UNIT_NONE && (unit->flags & MAP_UNIT_WAITED) == 0 &&
        Ov000_gUnitStats[unit->type].aiClass == AI_CLASS_DIRECT) {
      *gAiQueueCursor++ = i;
    }
  }
  *gAiQueueCursor = 0;
  gAiQueueCursor = gAiQueue;
  /* rain also brings fog */
  if (STATE->unk_1c4->fog != 0 || STATE->unk_1c4->weather == WEATHER_RAIN) {
    AiQueue_SortByVision(TRUE);
  } else {
    AiQueue_SortByMovement(FALSE);
  }
  gAiQueueRoutine = AiUnit_ActByOrderFog;
  gAiTurnState->state = AI_STATE_RUN_QUEUE;
}
