#include "types.h"
#include "game/task.h"

extern const TaskScriptCmd data_ov002_02369724[];
extern const TaskScriptCmd data_ov002_02369744[];
extern const TaskScriptCmd data_ov002_02369764[];
void Task_DestroyByScript(const TaskScriptCmd* script);

void func_ov002_0235ec84(void) {
  Task_DestroyByScript(data_ov002_02369724);
  Task_DestroyByScript(data_ov002_02369744);
  Task_DestroyByScript(data_ov002_02369764);
}
