#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"
#include "game/unit_list_task.h"

Task *Task_FindByScript(const void *script);
void Sound_PlaySe(u32 soundId);
void UnitList_Build(int sortMode, int reverse);
void UnitList_DrawRows(int scroll);

extern const TaskScriptCmd data_0215fad0[];

/* onConfirm of the four sort buttons (ids 248 to 251 pick sort mode 3 down to 0):
 * sets the mode, rebuilds the list and redraws the rows. */
void UnitList_OnSortButton(MenuButton *button)
{
    UnitListTask *task = (UnitListTask *)Task_FindByScript(data_0215fad0);

    if (task == NULL) {
        return;
    }
    switch (button->id) {
    case 251:
        task->sortMode = 0;
        break;
    case 250:
        task->sortMode = 1;
        break;
    case 249:
        task->sortMode = 2;
        break;
    case 248:
        task->sortMode = 3;
        break;
    }
    Sound_PlaySe(0x10002);
    UnitList_Build(task->sortMode, task->reverse);
    UnitList_DrawRows(task->scroll);
}
