#include "types.h"
#include "game/unit_list_task.h"

/* Moves the list down one row; FALSE when the last unit is already on screen. */
BOOL UnitList_ScrollDown(UnitListTask *list)
{
    if (list->topRow + UNIT_LIST_VISIBLE_ROWS < list->unitCount) {
        list->topRow++;
        return TRUE;
    }
    return FALSE;
}
