#include "types.h"
#include "game/task.h"

extern int data_0229eae4;
void DesignRoom_Exit(void);
void DesignRoom_Init(void);
void DesignRoom_Update(void);
void MenuDepth_Pop(void);
void MenuDepth_Push(void);
void Ov006_StartSlotsSlideIn(void);
extern u8 data_0216704c[];
void func_020d331c(void);
void func_ov006_0235068c(void);
void func_ov006_02350eb0(void);
void func_ov006_02352764(void);

static TaskScriptCmd data_ov006_0236a1f0[5] = {
#include "data/ov006/data_ov006_0236a1f0.inc.c"
};
TaskScriptCmd data_ov006_0236a218[8] = {
#include "data/ov006/data_ov006_0236a218.inc.c"
};
TaskScriptCmd data_ov006_0236a258[22] = {
#include "data/ov006/data_ov006_0236a258.inc.c"
};
TaskScriptCmd data_ov006_0236a308[32] = {
#include "data/ov006/data_ov006_0236a308.inc.c"
};

void func_ov006_02350ec0(void) {
  data_0229eae4 = 1;
  Task_Create(data_ov006_0236a1f0, TASK_LIST(3));
}
