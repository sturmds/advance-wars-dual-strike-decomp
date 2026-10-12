#include "types.h"
#include "game/task.h"
#include "game/menu_button.h"
#include "game/unit_list_task.h"

Task* Task_FindByScript(const void* script);

extern const TaskScriptCmd data_0215fad0[];
extern const MenuButtonTemplate data_0215f990;
extern const MenuButtonTemplate data_0215f9b8;
extern const MenuButtonTemplate data_0215f9e0;
extern const MenuButtonTemplate data_0215fa08;
extern const MenuButtonTemplate data_0215fa30;
extern const MenuButtonTemplate data_0215fa58;
extern const MenuButtonTemplate data_0215fa80;
extern const MenuButtonTemplate data_0215faa8;

/* Adds the buttons of the unit list menu: seven fixed ones (ids 248-254,
 * two of them at the top and bottom middle of the screen) and one per
 * visible list row (ids 0-5, 16 pixels apart from y 64), then selects row 0. */
void UnitList_AddButtons(void) {
  UnitListTask* task = (UnitListTask*)Task_FindByScript(data_0215fad0);
  int row;

  MenuCursor_SetRect(0, -16, 256, -16);
  MenuButton_Add(0, 0, &data_0215fa30, 254);
  MenuButton_Add(112, 16, &data_0215fa80, 253);
  MenuButton_Add(112, 160, &data_0215f9e0, 252);
  MenuButton_Add(0, 0, &data_0215f9b8, 251);
  MenuButton_Add(0, 0, &data_0215fa58, 250);
  MenuButton_Add(0, 0, &data_0215faa8, 249);
  MenuButton_Add(0, 0, &data_0215f990, 248);
  for (row = 0; row < task->unitCount; row++) {
    if (row == UNIT_LIST_VISIBLE_ROWS) {
      break;
    }
    MenuButton_Add(0, 64 + row * 16, &data_0215fa08, row);
  }
  MenuButton_Select(0);
}
