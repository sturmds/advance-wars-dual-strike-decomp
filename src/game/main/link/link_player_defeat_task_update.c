#include "types.h"
#include "game/task.h"
#include "game/Unk_02080440.h"
#include "game/map_unit.h"

extern Unk_02080440* data_027c027c;
extern int data_027c0284;
extern TaskScriptCmd data_021664e0[];

void Task_ResumeScript(Task* task);
Task* Task_FindByScript(void* script);
void LinkPlayerDefeat_StartUnitTask(MapUnit* unit, Task* parent);

#define STATE (data_027c027c->unk_48[data_027c0284])

/* Update of the task LinkPlayerDefeatTask_Create starts. For reason codes 1 and
 * 16 it ends at once. Otherwise it starts LinkPlayerDefeat_StartUnitTask on one unit slot of the
 * player per pass (unk_68 counts them), waiting while the previous unit's task
 * (data_021664e0) is still running, then resumes the script after 51 slots. */
void LinkPlayerDefeatTask_Update(Task* task) {
  s16 slot;

  if (task->unk_66 == 1) {
    Task_ResumeScript(task);
    return;
  }
  if (task->unk_66 == 16) {
    Task_ResumeScript(task);
    return;
  }
  if (Task_FindByScript(data_021664e0) != NULL) {
    return;
  }
  /* needed to match: a loop that always returns */
  while ((slot = task->unk_68) < 51) {
    MapUnit* unit =
        &STATE->units[Ov000_gPlayerFirstUnitSlot[task->unk_64] + slot];

    task->unk_68 = slot + 1;
    LinkPlayerDefeat_StartUnitTask(unit, task);
    return;
  }
  Task_ResumeScript(task);
}
