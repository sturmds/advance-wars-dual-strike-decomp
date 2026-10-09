#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

extern const TaskScriptCmd data_0215ad3c[];
extern const TaskScriptCmd data_0215ad6c[];
void Task_ResumeByScript(void *script);
Task *Task_Create(const TaskScriptCmd *script, void *parentOrList);

/* Resumes the tasks of data_0215ad6c, drops the selectable buttons, turns
 * touch input off and starts the data_0215ad3c script in task list 3. */
void func_02045b44(void)
{
    Task_ResumeByScript((void *)data_0215ad6c);
    MenuButtons_RemoveSelectable();
    MenuButtons_DisableTouch();
    Task_Create(data_0215ad3c, (void *)3);
}
