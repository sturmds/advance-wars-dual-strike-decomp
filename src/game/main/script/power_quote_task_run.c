#include "types.h"
#include "game/Unk_02080440.h"
#include "game/power_activate_task.h"

int CoQuote_StartRandom(int player);
BOOL CoQuote_StartMapLine(int player);

/* Update of the child task Power_StartTasks starts: shows the CO quote of the
 * player whose power was activated. In the campaign a map line goes first. */
void PowerQuoteTask_Run(PowerActivateTask* task) {
  int player = task->player;

  if (MAP_STATE->gameMode == GAME_MODE_CAMPAIGN) {
    if (CoQuote_StartMapLine(player)) {
      return;
    }
    CoQuote_StartRandom(player);
  } else {
    CoQuote_StartRandom(player);
  }
}
