#include "types.h"
#include "game/Unk_02080440.h"
#include "game/unit_show_entry.h"
#include "game/unit_show_task.h"

extern u32 gLinkPlay;

void func_020f5888(int x, int y);
void func_020b98e8(void);

/* Copies a path into the task, sets the script state and scrolls the camera
 * to the unit's tile unless unk_18 is 21 with gLinkPlay set. The odd
 * shape of the scroll test (a flag set to 0, then 1, ORed with a second test)
 * is needed to match. */
void UnitShowTask_SetPath(UnitShowTask* task, const u8* path) {
  const s8* steps = (const s8*)path;
  int i;
  int scroll;

  for (i = 0; i < UNIT_SHOW_PATH_LENGTH; i++) {
    task->script[i] = *steps++;
  }
  scroll = 0;
  task->scriptPos = scroll;
  task->state = UNIT_SHOW_STATE_SCRIPT;
  if (gLinkPlay == 0) {
    scroll = 1;
  }
  if (scroll | (gLinkPlay != 0 ? MAP_STATE->unk_18 != 21 : 0)) {
    func_020f5888((s16)(task->x / 16), (s16)(task->y / 16));
  }
  func_020b98e8();
  ShowEntry_Finish(task);
}
