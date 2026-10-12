#include "types.h"
#include "game/task.h"

void Task_DestroyByScript(const TaskScriptCmd* script);
void func_02027a6c(void);

extern const TaskScriptCmd data_ov003_02364b74[];
extern const TaskScriptCmd data_ov003_02364bb4[];
extern const TaskScriptCmd data_ov003_02364bf4[];
extern const TaskScriptCmd data_ov003_02364c34[];
extern const TaskScriptCmd data_ov003_02364c7c[];

/* Stops every task script the overlay started, then the shared ones of func_02027a6c. */
void Ov003_DestroyTasks(void) {
  Task_DestroyByScript(data_ov003_02364c7c);
  Task_DestroyByScript(data_ov003_02364b74);
  Task_DestroyByScript(data_ov003_02364bb4);
  Task_DestroyByScript(data_ov003_02364c34);
  Task_DestroyByScript(data_ov003_02364bf4);
  Task_DestroyByScript(data_ov003_02364c7c);
  func_02027a6c();
}
