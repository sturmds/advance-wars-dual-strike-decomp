#include "types.h"
#include "game/task.h"
#include "game/link_banner_task.h"

extern const TaskScriptCmd data_0215fee4[];

/* Starts the banner task for the button with this id. Button 4 gets text 78 at
 * x 99; the others get text 6 at x 243. The y position is 3 + 40 * id. The
 * position goes into the second and third fields, which LinkBanner_Init
 * and LinkBanner_ShrinkBox later reuse as width and timer. */
void LinkBanner_Create(int id) {
  LinkBannerTask* task =
      (LinkBannerTask*)Task_Create(data_0215fee4, TASK_LIST(4));

  if (id == 4) {
    task->textId = 78;
    task->timer = id * 40 + 3;
    task->width = 99;
    task->unk_38 = 25;
  } else {
    task->textId = 6;
    task->timer = id * 40 + 3;
    task->width = 243;
    task->unk_38 = 25;
  }
}
