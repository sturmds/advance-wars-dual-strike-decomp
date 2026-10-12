#include "types.h"
#include "game/co_rank_task.h"
#include "game/event_script.h"
#include "game/unlocks.h"

extern const EventScriptCommand data_0215b124[];

/* Once only (game flag 40): when a CO has reached rank 10, starts an event script. */
void CoRankTask_StartRank10Event(CoRankTask* task) {
  if (GameFlag_IsSet(40)) {
    return;
  }
  if (task->cos[0].rank >= 10 || task->cos[1].rank >= 10) {
    GameFlag_Set(40, 1);
    EventScript_Start(data_0215b124);
  }
}
