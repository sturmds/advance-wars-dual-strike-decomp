#include "types.h"
#include "game/strike_target.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/ex_machina_bolt_task.h"

void func_020f5888(int x, int y);

/* Script step of gExMachinaScript: picks a site for the bolt, scrolls the
 * view to it and stores it as the target; jumps to label 0 when there is no
 * site. */
void ExMachina_PickTarget(ExMachinaTask* task) {
  s16 site[2];

  if (Strike_FindBestSite(site, MAP_STATE->currentPlayer, 0,
                          STRIKE_MODE_RANDOM) == TRUE) {
    func_020f5888(site[0], site[1]);
    task->targetX = site[0];
    task->targetY = site[1];
  } else {
    Task_JumpToLabel((Task*)task, 0);
  }
}
