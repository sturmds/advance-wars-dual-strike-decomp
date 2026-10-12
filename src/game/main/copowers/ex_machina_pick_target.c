#include "types.h"
#include "game/strike_target.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/ex_machina_bolt_task.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;

void func_020f5888(int x, int y);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Script step of gExMachinaScript: picks a site for the bolt, scrolls the
 * view to it and stores it as the target; jumps to label 0 when there is no
 * site. */
void ExMachina_PickTarget(ExMachinaTask* task) {
  s16 site[2];

  if (Strike_FindBestSite(site, STATE->currentPlayer, 0, STRIKE_MODE_RANDOM) ==
      TRUE) {
    func_020f5888(site[0], site[1]);
    task->targetX = site[0];
    task->targetY = site[1];
  } else {
    Task_JumpToLabel((Task*)task, 0);
  }
}
