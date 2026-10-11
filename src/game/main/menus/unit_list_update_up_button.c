#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"
#include "game/unit_list_task.h"

Task *Task_FindByScript(const void *script);
BOOL UnitList_ScrollUp(UnitListTask *task);

extern const TaskScriptCmd data_0215fad0[];

/* onUpdate of the scroll-up button (id 253). Touching it scrolls the list one
 * row at once, then every 4 frames after a first wait of 10 frames. */
void UnitList_UpdateUpButton(MenuButton *button)
{
    UnitListTask *task = (UnitListTask *)Task_FindByScript(data_0215fad0);
    int scrollNow = 0;
    int touching = 0;
    int wasTouching;

    if (task == NULL) {
        return;
    }
    if ((u8)(button->touchState + 255) <= 1) {
        touching = 1;
    }
    wasTouching = task->unk_34;
    if (wasTouching == 0) {
        if (touching == 1) {
            scrollNow = 1;
        }
    }
    if (touching == 1 && wasTouching == 1) {
        task->unk_2f--;
        if (task->unk_2f == 0) {
            task->unk_2f = 4;
            scrollNow = 1;
        }
    } else {
        task->unk_2f = 10;
    }
    task->unk_34 = touching;
    if (scrollNow == 1) {
        UnitList_ScrollUp(task);
    }
}
