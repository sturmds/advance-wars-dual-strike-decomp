#include "types.h"
#include "game/task.h"

void func_0201e954(void);
void func_ov006_023505c4(void);
void MenuDepth_Pop(void);
void func_ov006_0235434c(void);
void func_ov006_023543c4(void);

static TaskScriptCmd data_ov006_0236abc4[7] = {
#include "data/ov006/data_ov006_0236abc4.inc.c"
};

void func_ov006_023542a8(void) {
  func_0201e954();
  Task_Create(data_ov006_0236abc4, TASK_LIST(3));
  func_ov006_023505c4();
}
