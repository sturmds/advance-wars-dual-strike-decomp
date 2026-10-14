#include "types.h"
#include "game/task.h"

void func_0201e954(void);
void func_ov006_023505c4(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void func_ov006_023540ec(void);
void func_ov006_0235436c(void);
void func_ov006_023543c4(void);

static TaskScriptCmd data_ov006_0236ac34[] = {
#include "data/ov006/data_ov006_0236ac34.inc.c"
};
u32 data_ov006_0236ac74[] = {
#include "data/ov006/data_ov006_0236ac74.inc.c"
};

void func_ov006_02354300(void) {
  func_0201e954();
  Task_Create(data_ov006_0236ac34, TASK_LIST(3));
  func_ov006_023505c4();
}
