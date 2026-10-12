#include "types.h"
#include "game/task.h"

void Task_DestroyByScript(const TaskScriptCmd* script);

extern const TaskScriptCmd data_02169c94[];
extern const TaskScriptCmd data_02169cac[];

/* Destroys the spawner task and the four letter tasks. */
void Splash_DestroyTasks(void) {
  Task_DestroyByScript(data_02169c94);
  Task_DestroyByScript(data_02169cac);
  Task_DestroyByScript(data_02169cac);
  Task_DestroyByScript(data_02169cac);
  Task_DestroyByScript(data_02169cac);
}
