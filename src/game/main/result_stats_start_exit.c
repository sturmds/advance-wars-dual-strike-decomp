#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"

void func_020077a4(int value);

extern const TaskScriptCmd data_0215a628[];

/* Closes the back button and starts the exit script of the stats screen. */
void ResultStats_StartExit(void)
{
    func_020077a4(0);
    MenuButton_Close(0);
    MenuButtons_DisableTouch();
    Task_Create(data_0215a628, TASK_LIST(3));
}
