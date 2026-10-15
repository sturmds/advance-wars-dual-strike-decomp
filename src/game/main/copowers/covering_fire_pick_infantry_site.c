#include "types.h"
#include "game/covering_fire_task.h"
#include "game/strike_target.h"
#include "game/task.h"
#include "game/Unk_02080440.h"

/* Script step of gCoveringFireScript: picks the site that hurts the most
 * enemy infantry and stores it as the target; jumps to label 0 when there
 * is no site. */
void CoveringFire_PickInfantrySite(CoveringFireTask* task) {
  s16 site[2];

  if (Strike_FindBestSite(site, MAP_STATE->currentPlayer, 0,
                          STRIKE_MODE_INFANTRY)) {
    task->targetX = site[0];
    task->targetY = site[1];
  } else {
    Task_JumpToLabel((Task*)task, 0);
  }
}
