#include "types.h"
#include "game/task.h"

Task* Task_FindByScript(const TaskScriptCmd* script);

void Battle_FadeInStep(void);
void Battle_FadeOutStep(void);
void Battle_StartFadeOut(void);
void func_020266b4(void);
void func_020dc71c(void);
void func_ov002_02350d20(void);

TaskScriptCmd data_ov002_02360044[2] = {
#include "data/ov002/data_ov002_02360044.inc.c"
};
static TaskScriptCmd data_ov002_02360054[8] = {
#include "data/ov002/data_ov002_02360054.inc.c"
};

/* Creates the task of data_ov002_02360054 unless the task of
 * data_ov002_02360044 (made by func_ov002_02350e1c) already runs. */
void Battle_EnsureTask(void) {
  if (Task_FindByScript(data_ov002_02360044) != NULL) {
    return;
  }
  Task_Create(data_ov002_02360054, TASK_LIST(1));
}
