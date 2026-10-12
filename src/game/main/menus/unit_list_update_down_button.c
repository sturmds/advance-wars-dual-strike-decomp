#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"
#include "game/unit_list_task.h"

Task* Task_FindByScript(const void* script);
BOOL UnitList_ScrollDown(UnitListTask* task);

extern const TaskScriptCmd data_0215fad0[];

/* onUpdate of the scroll-down button (id 252). Touching it scrolls the list one
 * row at once, then every 4 frames after a first wait of 10 frames. */
void UnitList_UpdateDownButton(MenuButton* button) {
  UnitListTask* task = (UnitListTask*)Task_FindByScript(data_0215fad0);
  int scrollNow = 0;
  int touching = 0;
  int wasTouching;

  if (task == NULL) {
    return;
  }
  /* touchState 1 or 2: pressed or held. */
  if ((u8)(button->touchState + 255) <= 1) {
    touching = 1;
  }
  wasTouching = task->downTouched;
  if (wasTouching == 0) {
    if (touching == 1) {
      scrollNow = 1;
    }
  }
  if (touching == 1 && wasTouching == 1) {
    task->downRepeatDelay--;
    if (task->downRepeatDelay == 0) {
      task->downRepeatDelay = 4;
      scrollNow = 1;
    }
  } else {
    task->downRepeatDelay = 10;
  }
  task->downTouched = touching;
  if (scrollNow == 1) {
    UnitList_ScrollDown(task);
  }
}
