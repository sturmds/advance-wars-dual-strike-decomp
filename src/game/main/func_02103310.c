#include "types.h"
#include "game/task.h"

void CoSelect_ResetToUnlockedCos(void);
void func_02070238(void);
void func_0208b988(void);
void func_020fc160(void);
void func_020fd8bc(void);
void func_020fd8d4(void);
void func_020fd904(void);
void func_020fd91c(void);
void func_02103714(void);

TaskScriptCmd data_0216d674[3] = {
#include "data/main/data_0216d674.inc.c"
};
TaskScriptCmd data_0216d65c[3] = {
#include "data/main/data_0216d65c.inc.c"
};
TaskScriptCmd data_0216d644[3] = {
#include "data/main/data_0216d644.inc.c"
};
TaskScriptCmd data_0216d62c[3] = {
#include "data/main/data_0216d62c.inc.c"
};
TaskScriptCmd data_0216d614[3] = {
#include "data/main/data_0216d614.inc.c"
};
static TaskScriptCmd data_0216d5fc[3] = {
#include "data/main/data_0216d5fc.inc.c"
};
TaskScriptCmd data_0216d68c[3] = {
#include "data/main/data_0216d68c.inc.c"
};

Task* func_02103310(void) {
  CoSelect_ResetToUnlockedCos();
  return Task_Create(data_0216d5fc, TASK_LIST(3));
}
