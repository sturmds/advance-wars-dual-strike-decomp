#include "types.h"
#include "game/task.h"

void func_0201e954(void);
void func_ov006_023505c4(void);
extern const TaskScriptCmd data_ov006_0236ab8c[];

void func_ov006_02354250(void) {
  func_0201e954();
  Task_Create(data_ov006_0236ab8c, TASK_LIST(3));
  func_ov006_023505c4();
}
