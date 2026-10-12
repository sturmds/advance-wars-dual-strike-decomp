#include "types.h"
#include "game/task.h"
#include "game/keypad.h"
#include "game/menu_button.h"
#include "game/unit_list_task.h"

Task* Task_FindByScript(const void* script);
void Sound_PlaySe(u32 soundId);
void UnitList_Build(int sortMode, int reverse);
void UnitList_Close(MenuButton* button);
void UnitList_PickRow(MenuButton* button);
BOOL UnitList_ScrollDown(UnitListTask* task);
BOOL UnitList_ScrollUp(UnitListTask* task);
void UnitList_DrawRows(int scroll);

extern const TaskScriptCmd data_0215fad0[];

/* Key handler of the unit list menu's row buttons. Up and Down move the
 * cursor, scrolling the list (UnitList_ScrollUp, UnitList_ScrollDown) at the top and
 * bottom rows; Left and Right step through the 4 sort modes, Select or
 * Start reverses the order, and each of those rebuilds and redraws the list.
 * A picks the row, B closes the menu. Right plays its sound twice. */
void UnitList_HandleKeys(MenuButton* button) {
  UnitListTask* task = (UnitListTask*)Task_FindByScript(data_0215fad0);
  int row = button->id;

  if (gKeypad->repeat & PAD_KEY_UP) {
    if (row >= 2) {
      row--;
    } else if (!UnitList_ScrollUp(task) && row != 0) {
      row--;
    }
  }
  if (gKeypad->repeat & PAD_KEY_DOWN) {
    if (row <= 3) {
      if (MenuButton_Find(row + 1) != NULL) {
        row++;
      }
    } else if (!UnitList_ScrollDown(task)) {
      if (MenuButton_Find(row + 1) != NULL) {
        row++;
      }
    }
  }
  if (row != button->id) {
    button = MenuButton_Select(row);
  }
  if (gKeypad->repeat & PAD_KEY_LEFT) {
    if (task->sortMode == 0) {
      task->sortMode = 3;
    } else {
      task->sortMode--;
    }
    Sound_PlaySe(0x10002);
    UnitList_Build(task->sortMode, task->reverse);
    UnitList_DrawRows(task->scroll);
  }
  if (gKeypad->repeat & PAD_KEY_RIGHT) {
    Sound_PlaySe(0x10002);
    if (task->sortMode == 3) {
      task->sortMode = 0;
    } else {
      task->sortMode++;
    }
    Sound_PlaySe(0x10002);
    UnitList_Build(task->sortMode, task->reverse);
    UnitList_DrawRows(task->scroll);
  }
  if (gKeypad->pressed & (PAD_BUTTON_SELECT | PAD_BUTTON_START)) {
    task->reverse++;
    task->reverse &= 1;
    Sound_PlaySe(0x10002);
    UnitList_Build(task->sortMode, task->reverse);
    UnitList_DrawRows(task->scroll);
  }
  if (gKeypad->pressed & PAD_BUTTON_A) {
    UnitList_PickRow(button);
  } else if (gKeypad->pressed & PAD_BUTTON_B) {
    UnitList_Close(NULL);
  }
}
