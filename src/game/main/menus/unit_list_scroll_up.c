#include "types.h"
#include "game/unit_list_task.h"

/* Moves the list up one row; FALSE when it is at the top. */
BOOL UnitList_ScrollUp(UnitListTask* list) {
  u8 topRow = list->topRow;

  if (topRow != 0) {
    int row = topRow - 1;
    list->topRow = row;
    return TRUE;
  }
  return FALSE;
}
