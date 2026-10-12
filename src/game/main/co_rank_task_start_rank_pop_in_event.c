#include "types.h"
#include "game/co_rank_task.h"
#include "game/event_script.h"
#include "game/unlocks.h"

extern const EventScriptCommand data_0215b0a4[];

/* Once only (game flag 39): when the rank digits of either CO have started
 * their pop-in (frame is nonzero), starts an event script. */
void CoRankTask_StartRankPopInEvent(CoRankTask* task) {
  if (GameFlag_IsSet(39)) {
    return;
  }
  if (task->cos[0].frame != 0 || task->cos[1].frame != 0) {
    GameFlag_Set(39, 1);
    EventScript_Start(data_0215b0a4);
  }
}
