#include "types.h"
#include "game/co_rank_task.h"
#include "game/event_script.h"
#include "game/unlocks.h"

extern const EventScriptCommand data_0215b0e4[];

/* Once only (game flag 66): when a CO has reached rank 100, starts an event script. */
void CoRankTask_StartMaxRankEvent(CoRankTask* task) {
  if (GameFlag_IsSet(66)) {
    return;
  }
  if (task->cos[0].rank == 100 || task->cos[1].rank == 100) {
    GameFlag_Set(66, 1);
    EventScript_Start(data_0215b0e4);
  }
}
