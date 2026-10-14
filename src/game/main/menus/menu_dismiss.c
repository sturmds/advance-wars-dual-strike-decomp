#include "types.h"
#include "game/task.h"

void Task_ResumeByScript(const void* script);
void MenuButtons_CloseAll(void);
extern TaskScriptCmd data_0215af10[];
extern const TaskScriptCmd data_0215aed0[];

void Menu_Dismiss(void) {
  Task_ResumeByScript(data_0215af10);
  MenuButtons_CloseAll();
  Task_Create(data_0215aed0, TASK_LIST(3));
}
